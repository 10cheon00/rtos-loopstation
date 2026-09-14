"""Private Slack mentions -> subscription-authenticated Codex CLI -> Slack thread."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import fcntl
import getpass
import json
import logging
import os
from pathlib import Path
import re
import shutil
import sqlite3
import subprocess
import tempfile
import threading

ROOT = Path(__file__).resolve().parent
STATE = ROOT / '.local'
CONFIG = STATE / 'config.json'
LOG = logging.getLogger('rtos-loopstation-PM')


def private_state():
    STATE.mkdir(mode=0o700, exist_ok=True)
    STATE.chmod(0o700)


def codex_env():
    # Never forward Slack secrets, API keys or parent agent execution context.
    return {k: v for k, v in os.environ.items()
            if k in {'HOME', 'PATH', 'TMPDIR', 'LANG', 'LC_ALL', 'CODEX_HOME'}}


def codex_binary():
    binary = shutil.which('codex')
    if not binary:
        raise RuntimeError('Codex CLI가 없습니다. codex 설치 후 다시 실행하세요.')
    return binary


def check_auth():
    result = subprocess.run([codex_binary(), 'login', 'status'], env=codex_env(),
                            capture_output=True, text=True, timeout=20)
    if result.returncode or 'logged in using chatgpt' not in (result.stdout + result.stderr).lower():
        raise RuntimeError('ChatGPT 인증이 확인되지 않아 중단했습니다. 터미널에서 codex login을 실행하세요.')


def generate(prompt):
    check_auth()
    # A fresh directory outside the repository avoids project hooks/configuration.
    with tempfile.TemporaryDirectory(prefix='loopstation-pm-') as work:
        reply = Path(work) / 'reply.txt'
        command = [codex_binary(), 'exec', '--ignore-user-config', '--ignore-rules',
                   '--skip-git-repo-check', '--ephemeral', '--sandbox', 'read-only',
                   '--color', 'never', '-C', work, '-o', str(reply)]
        settings = {
            'forced_login_method': 'chatgpt', 'model_provider': 'openai',
            'approval_policy': 'never', 'web_search': 'disabled',
            'model_instructions_file': str(ROOT / 'pm-instructions.txt'),
            'features.shell_tool': False, 'features.unified_exec': False,
            'features.hooks': False, 'features.plugins': False,
            'features.multi_agent': False, 'features.memories': False,
        }
        for key, value in settings.items():
            command.extend(['-c', key + '=' + json.dumps(value)])
        command.append('-')
        result = subprocess.run(command, input=prompt, text=True, env=codex_env(),
                                capture_output=True, timeout=180, cwd=work)
        if result.returncode or not reply.exists():
            # Raw CLI stderr can contain private data; keep it out of Slack/logs.
            raise RuntimeError('Codex 응답 생성 실패: 로그인 상태, 구독 한도, CLI 버전을 확인하세요.')
        answer = reply.read_text().strip()
        if not answer:
            raise RuntimeError('Codex가 빈 답변을 반환했습니다.')
        return answer[:2900]


class Store:
    def __init__(self, path):
        self.path = path
        with self.connect() as db:
            db.execute('CREATE TABLE IF NOT EXISTS jobs '
                       '(id TEXT PRIMARY KEY, channel TEXT, thread TEXT, question TEXT, '
                       'answer TEXT, status TEXT, created TEXT DEFAULT CURRENT_TIMESTAMP)')

    def connect(self):
        return sqlite3.connect(self.path, timeout=10)

    def claim(self, eid, channel, thread, question):
        with self.connect() as db:
            return db.execute('INSERT OR IGNORE INTO jobs(id,channel,thread,question,status) '
                              'VALUES(?,?,?,?,?)', (eid, channel, thread, question, 'pending')).rowcount == 1

    def update(self, eid, status, answer=None):
        with self.connect() as db:
            db.execute('UPDATE jobs SET status=?,answer=COALESCE(?,answer) WHERE id=?',
                       (status, answer, eid))

    def history(self, channel, thread):
        with self.connect() as db:
            rows = db.execute('SELECT question,answer FROM jobs WHERE channel=? AND thread=? '
                              "AND status='sent' ORDER BY rowid DESC LIMIT 6", (channel, thread)).fetchall()
        return [{'user': q, 'pm': a} for q, a in reversed(rows)]


def build_prompt(store, channel, thread, question):
    return json.dumps({'project_context': (ROOT / 'project-context.txt').read_text()[:6000],
                       'recent_thread': store.history(channel, thread),
                       'current_message': question}, ensure_ascii=False)


def eligible(event, cfg):
    return (event.get('type') == 'app_mention' and not event.get('bot_id')
            and not event.get('subtype') and event.get('user') == cfg['user_id']
            and event.get('channel') == cfg['channel_id'])


def setup():
    private_state()
    if CONFIG.exists():
        raise RuntimeError('설정 파일이 이미 있습니다. .local/config.json을 로컬 편집기로 수정하세요.')
    check_auth()
    cfg = {'bot_token': getpass.getpass('Bot Token (xoxb-): ').strip(),
           'app_token': getpass.getpass('App Token (xapp-): ').strip(),
           'user_id': input('본인의 Slack 멤버 ID (U... 또는 W...): ').strip(),
           'channel_id': input('허용할 채널 ID (C... 또는 G...): ').strip()}
    if not (cfg['bot_token'].startswith('xoxb-') and cfg['app_token'].startswith('xapp-')
            and re.fullmatch(r'[UW][A-Z0-9]+', cfg['user_id'])
            and re.fullmatch(r'[CG][A-Z0-9]+', cfg['channel_id'])):
        raise RuntimeError('입력 형식을 확인하세요. 설정은 저장하지 않았습니다.')
    with CONFIG.open('x') as stream:
        json.dump(cfg, stream, indent=2)
    CONFIG.chmod(0o600)
    print('설정 저장 완료. run.sh doctor로 확인한 뒤 run.sh run으로 실행하세요.')


def run(cfg):
    from slack_bolt import App
    from slack_bolt.adapter.socket_mode import SocketModeHandler
    from slack_sdk import WebClient

    check_auth()
    lock = (STATE / 'process.lock').open('w')
    try:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        raise RuntimeError('PM이 이미 실행 중입니다.') from None
    store = Store(STATE / 'history.sqlite3')
    # No transparent POST retries: an ambiguous response must not duplicate a message.
    client = WebClient(token=cfg['bot_token'], retry_handlers=[])
    app = App(client=client)
    identity = client.auth_test()
    LOG.info('봇 연결: name=%s bot_user_id=%s 허용 user=%s channel=%s',
             identity.get('user'), identity.get('user_id'),
             cfg['user_id'], cfg['channel_id'])
    pool = ThreadPoolExecutor(max_workers=1)
    slots = threading.BoundedSemaphore(5)

    def post(channel, thread, text):
        client.chat_postMessage(channel=channel, thread_ts=thread, text=text,
                                blocks=[{'type': 'section', 'text': {
                                    'type': 'plain_text', 'text': text}}],
                                parse='none', link_names=False,
                                unfurl_links=False, unfurl_media=False)

    def process(eid, channel, thread, question):
        try:
            LOG.info('Codex 응답 생성 시작 event=%s', eid)
            answer = generate(build_prompt(store, channel, thread, question))
            store.update(eid, 'generated', answer)
            try:
                post(channel, thread, answer)
            except Exception:
                store.update(eid, 'delivery_unknown')
                LOG.error('Slack 전송 결과 불명확: 자동 재생성/재전송하지 않습니다. event=%s', eid)
                return
            store.update(eid, 'sent')
            LOG.info('답변 완료 event=%s', eid)
        except Exception as exc:
            store.update(eid, 'failed')
            LOG.error('답변 생성 실패 (%s) event=%s', type(exc).__name__, eid)
            try:
                post(channel, thread, 'PM 응답 생성에 실패했습니다. 로컬 터미널에서 doctor를 실행해 '
                     '로그인·구독 한도·네트워크를 확인한 뒤 다시 멘션해 주세요.')
            except Exception:
                LOG.error('오류 안내 전송도 실패했습니다.')
        finally:
            slots.release()

    @app.event('app_mention')
    def mention(event, body):
        LOG.info('멘션 수신 event=%s user=%s channel=%s', body.get('event_id'),
                 event.get('user'), event.get('channel'))
        if not eligible(event, cfg):
            LOG.warning('멘션 제외: 허용 사용자/채널 또는 이벤트 유형 불일치')
            return
        eid = body.get('event_id')
        if not eid:
            return
        channel = event['channel']
        thread = event.get('thread_ts') or event['ts']
        question = re.sub(r'<@[A-Z0-9]+>', '', event.get('text', '')).strip()
        if not question or len(question) > 4000:
            post(channel, thread, '멘션 뒤에 1~4000자 이내의 질문을 입력해 주세요.')
            return
        if not store.claim(eid, channel, thread, question):
            LOG.info('중복 이벤트 제외 event=%s', eid)
            return
        if not slots.acquire(blocking=False):
            store.update(eid, 'busy')
            post(channel, thread, 'PM 요청이 밀려 있습니다. 잠시 후 다시 멘션해 주세요.')
            return
        pool.submit(process, eid, channel, thread, question)

    print('rtos-loopstation-PM 실행 중. 허용된 사용자/채널의 멘션에만 답합니다. 종료: Ctrl+C', flush=True)
    handler = SocketModeHandler(app, cfg['app_token'])
    try:
        handler.start()
    finally:
        handler.close()
        pool.shutdown(wait=True, cancel_futures=True)
        lock.close()


def main():
    os.umask(0o077)
    logging.basicConfig(level=logging.INFO, format='%(asctime)s %(levelname)s %(message)s')
    parser = argparse.ArgumentParser()
    parser.add_argument('mode', choices=['setup', 'doctor', 'smoke', 'run'])
    args = parser.parse_args()
    private_state()
    if args.mode == 'setup':
        setup()
    elif args.mode == 'smoke':
        print(generate('프로젝트 PM으로서 첫 인사를 한국어 한 문장으로 해주세요.'))
    elif args.mode == 'doctor':
        check_auth()
        print('Codex ChatGPT 인증: 정상')
        print('Slack 설정: ' + ('있음' if CONFIG.exists() else '없음 — setup 필요'))
        if CONFIG.exists():
            from slack_sdk import WebClient
            cfg = json.loads(CONFIG.read_text())
            result = WebClient(token=cfg['bot_token']).auth_test()
            print('Slack Bot 인증: ' + ('정상' if result.get('ok') else '실패'))
            print('실제 봇 이름/사용자 ID:', result.get('user'), result.get('user_id'))
            scopes = result.headers.get('x-oauth-scopes', '')
            granted = {scope.strip() for scope in scopes.split(',')}
            for scope in ('app_mentions:read', 'chat:write'):
                print(f'{scope}: ' + ('정상' if scope in granted else '권한 확인 필요'))
            connection = WebClient().apps_connections_open(app_token=cfg['app_token'])
            print('App Token / Socket Mode 연결 발급: ' + ('정상' if connection.get('ok') else '실패'))
            print('허용 사용자/채널:', cfg['user_id'], cfg['channel_id'])
        print('doctor는 모델 호출/게시를 하지 않습니다. Enable Events가 On이고 app_mention을 구독했는지도 확인하세요.')
    else:
        if not CONFIG.exists():
            raise RuntimeError('먼저 run.sh setup을 실행하세요.')
        run(json.loads(CONFIG.read_text()))


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('\nPM 종료')
    except Exception as error:
        # Do not print arbitrary exceptions from SDKs (may embed request data).
        print(str(error) if isinstance(error, RuntimeError) else
              f'실행 실패 ({type(error).__name__}). 설정과 네트워크를 확인하세요.')
        raise SystemExit(1)
