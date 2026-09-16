---
title: rtos-loopstation-PM 로컬 Slack 연결 사용 안내
version: 0.3.1
change_history:
  - date: 2026-09-15
    version: 0.3.0
    summary: 멘션으로 등록한 스레드의 일반 댓글 응답과 중복 방지 및 추가 권한 설정을 안내함
  - date: 2026-09-14
    version: 0.2.0
    summary: 실제 앱 이름을 반영하고 이벤트 활성화 장애 원인과 토큰 권한 진단·수신 로그 안내를 추가함
  - date: 2026-09-14
    version: 0.1.0
    summary: ChatGPT 인증 Codex CLI 기반 PM 봇의 설치·실행·검증 및 운영 제약을 문서화함
  - date: 2026-09-16
    version: 0.3.1
    summary: 문서의 설계 및 설명 내용을 갱신함
---

# rtos-loopstation-PM — Slack과 Codex CLI 연결

기존 rtos-loopstation-PM Slack 앱의 멘션을 Mac에서 받아, ChatGPT로 로그인한 Codex CLI로
답변을 생성하고 같은 Slack 스레드에 게시한다. 별도 OpenAI API 키는 사용하지 않는다.
Codex 구독 사용량을 소비하며, 한도가 소진되면 실패한다. API 키로 전환하거나 크레딧을
구매하는 기능은 없다. 로컬 프로그램이지만 Slack 통신과 모델 추론은 인터넷을 사용한다.

## 준비

- Codex CLI 0.149.1에서 확인한 옵션을 사용한다. `codex login status`가
  `Logged in using ChatGPT`인지 확인한다. 로그인하지 않았다면 `codex login`을 실행한다.
- Python 3.10 이상과 인터넷 연결이 필요하다.
- 기존 Slack 앱에서 Socket Mode를 활성화한다.
- Bot Token Scopes: `app_mentions:read`, `chat:write`, `channels:history`.
- Event Subscriptions: Enable Events, Subscribe to bot events에 `app_mention`, `message.channels` 추가.
- App-Level Token에는 `connections:write` 권한을 부여한다.
- 앱을 워크스페이스에 설치하고 사용할 채널에 초대한다. 권한 변경 후에는 재설치한다.

## 최초 설정

터미널에서 실행한다. 첫 실행은 프로젝트 전용 가상환경과 Slack 라이브러리를 설치한다.

```bash
cd /Users/kimtaekcheon/EmbeddedProjects/rtos-loopstation
bash tools/loopstation-pm/run.sh setup
```

네 항목을 순서대로 입력한다. 토큰 입력은 화면에 표시되지 않는다.

1. Bot Token: Slack 앱의 OAuth & Permissions에 있는 `xoxb-...`.
2. App Token: Basic Information의 App-Level Tokens에서 발급한 `xapp-...`.
3. 본인의 멤버 ID: Slack에서 본인 프로필을 열고 더 보기 메뉴의 멤버 ID 복사를 사용한다.
   보통 `U`로 시작한다. 표시 이름이나 이메일 주소가 아니다.
4. 채널 ID: 채널 이름을 클릭해 채널 상세 정보에서 채널 ID를 복사한다. `C` 또는 `G`로 시작한다.

설정은 `.local/config.json`에 저장한다. `.local/`은 Git에서 제외되며 디렉터리는 700,
설정 파일은 600 권한을 사용한다. 설정 변경은 이 파일을 로컬 편집기에서 수정한다.
토큰과 Codex 인증 파일을 Slack, 채팅, Git에 공유하지 않는다.

## 확인과 실행

```bash
bash tools/loopstation-pm/run.sh doctor
bash tools/loopstation-pm/run.sh smoke
bash tools/loopstation-pm/run.sh run
```

- `doctor`: Codex 로그인, 실제 봇 ID, Bot 토큰의 멘션·게시 권한, App 토큰의 Socket Mode 연결 발급을 확인한다. 모델 호출이나 Slack 게시 없음.
- `smoke`: Codex가 한 문장 답변을 생성하는지 확인한다. 구독 사용량을 소비하며 Slack 게시 없음.
- `run`: Socket Mode 연결을 유지하고 실제 멘션을 처리한다. 종료는 Ctrl+C.
  처리 중인 응답이 있다면 종료까지 최대 180초 정도 기다릴 수 있다.

Slack의 허용 채널에서 실제 앱을 선택하여 멘션한다.

> @rtos-loopstation-PM 이번 주에는 4시간을 쓸 수 있어. 이번 작업의 범위부터 함께 정하자.

처음에 앱을 멘션하면 해당 스레드가 등록되며, 이후 같은 스레드의 본인 댓글에는 멘션 없이 답한다.
기존 스레드는 업데이트 후 한 번 다시 멘션해 등록한다. 등록 상태는 재시작 후에도 유지된다.
일반 채널 글, 미등록 스레드, 다른 사용자와 봇의 댓글에는 답하지 않는다. 첨부파일과 링크 내용은 읽지 않는다. Mac이 잠자거나 프로그램이 꺼지면 응답할 수 없으며, 꺼진 동안의
멘션을 모두 복구한다는 보장은 없다. 화면 잠금과 시스템 잠자기는 다르다.

## 기억과 PM 역할

`pm-instructions.txt`는 PM 역할 지침, `project-context.txt`는 직접 제공할 프로젝트 개요다.
처음에는 저장소를 자동 분석하지 않는다. Codex 앱의 기존 대화도 공유되지 않는다.
텍스트 파일을 수정하면 다음 요청부터 반영된다.

스레드별 대화는 `.local/history.sqlite3`에 저장된다. 최근 성공한 6회 질문·답변만 다음
요청에 전달하며, 다른 스레드는 섞지 않는다. 재시작 후에도 DB는 유지된다. 오래된 합의나
스레드 간 공유가 필요한 사항은 `project-context.txt`에 따로 반영한다. 무제한 장기 기억이나
자동 일정 관리 기능은 없다.

## 실행 범위와 오류 처리

- 지정한 사용자와 채널의 멘션 및 등록된 스레드 댓글만 허용하며 한 번에 하나씩, 최대 5개 요청을 처리/대기한다.
- 질문은 4000자 이내, 응답은 2900자 이내다. 응답이 길면 잘릴 수 있다.
- 채널 ID와 메시지 ts를 DB 키로 사용하며, 멘션의 message 이벤트는 app_mention 경로에 맡겨 이중 응답을 막는다.
- Codex는 저장소 밖의 임시 디렉터리에서 read-only, approval never로 실행한다.
  사용자 설정을 로드하지 않고 셸, 웹 검색, 플러그인, hook, 다중 에이전트를 비활성화한다.
  PM은 전달받은 텍스트만으로 답변하며 펌웨어 파일을 직접 수정하지 않는다.
- Codex 자식 프로세스에는 Slack 토큰과 API 키 환경변수를 전달하지 않는다.
  ChatGPT 인증을 매번 확인하고 해당 로그인 방식을 강제한다.
- Slack 답변은 plain_text로 게시해 모델 출력의 멘션이나 링크 서식을 실행하지 않는다.
- 생성 실패는 자동 재시도하지 않는다. 상태를 확인한 뒤 새 멘션으로 요청한다.
- 생성 후 전송 결과가 불명확하면 DB에 `delivery_unknown`으로 답변을 보존하고
  중복 게시를 피하려고 자동 재생성/재전송하지 않는다. 실제 스레드를 확인한다.
- 강제 종료된 `pending` 작업도 자동 재생하지 않는다. 필요한 요청은 새로 멘션한다.
- 터미널 로그는 메시지 본문이나 원시 Codex 오류 대신 이벤트 ID와 오류 종류를 표시한다.

## 문제 해결

| 증상 | 확인할 것 |
|---|---|
| Slack에 앱은 있지만 답이 없음 | 프로그램 실행, Socket Mode, 이벤트 구독, 채널 초대, 사용자·채널 ID |
| `doctor` 성공인데 멘션 수신 실패 | App Token의 `connections:write`, Socket Mode, `app_mention` 구독 |
| ChatGPT 인증 확인 실패 | 일반 터미널에서 `codex login`, `codex login status` |
| Codex 생성 실패 | `smoke` 실행, 구독 한도, 네트워크, CLI 버전 |
| 다음 메시지를 기억하지 못함 | 같은 스레드인지, 업데이트 후 한 번 멘션해 등록했는지 |
| 이미 실행 중 | 기존 터미널의 PM 종료 후 재실행 |

## 검증

```bash
cd /Users/kimtaekcheon/EmbeddedProjects/rtos-loopstation/tools/loopstation-pm
.venv/bin/python -m unittest -v
```

단위 테스트는 인증 방식 제한, 자격 증명 미전달, 셸 삽입 방지, 이벤트 중복 제거,
대화의 재시작 후 복구와 스레드 분리, 기록 길이 제한을 검증한다.
2026-09-14 23:16 KST에 team-room의 실제 사용자 멘션이 수신되고 Codex 답변이
동일 스레드에 게시되는 왕복 테스트를 통과했다.

## 공식 자료

- [Codex 인증](https://learn.chatgpt.com/docs/auth)
- [Codex 비대화형 실행](https://learn.chatgpt.com/docs/non-interactive-mode)
- [Slack Bolt 설정](https://docs.slack.dev/tools/bolt-python/creating-an-app/)

## 2026-09-14 멘션 왕복 장애 수정

Event Subscriptions의 Enable Events가 Off이면 app_mention이 목록에 있어도 이벤트가 전달되지 않는다. On으로 바꾼 뒤 저장해야 한다. PM 프로세스도 실행 중이어야 한다. 터미널에는 봇 이름·ID, 멘션 수신, 허용 조건에 따른 제외, Codex 생성 시작, 게시 완료를 표시한다. 표시 이름과 API 사용자 이름은 다를 수 있으며 연결은 ID를 기준으로 한다.
