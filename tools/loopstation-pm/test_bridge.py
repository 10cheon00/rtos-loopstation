import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
import subprocess

import bridge


class BridgeTests(unittest.TestCase):
    def test_restrict_sender_and_channel(self):
        cfg = {'user_id': 'U1', 'channel_id': 'C1'}
        event = {'type': 'app_mention', 'user': 'U1', 'channel': 'C1'}
        self.assertTrue(bridge.eligible(event, cfg))
        for delta in [{'user': 'U2'}, {'channel': 'C2'}, {'bot_id': 'B1'},
                      {'subtype': 'message_changed'}, {'type': 'message'}]:
            self.assertFalse(bridge.eligible(dict(event, **delta), cfg))

    def test_dedup_and_persistent_thread_isolation(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'history.db'
            store = bridge.Store(path)
            self.assertTrue(store.claim('E1', 'C1', 'T1', '목표'))
            self.assertFalse(store.claim('E1', 'C1', 'T1', '중복'))
            store.update('E1', 'generated', '답변')
            self.assertEqual(store.history('C1', 'T1'), [])
            store.update('E1', 'sent')
            restored = bridge.Store(path)
            self.assertEqual(restored.history('C1', 'T1'), [{'user': '목표', 'pm': '답변'}])
            self.assertEqual(restored.history('C1', 'T2'), [])

    def test_history_bounded(self):
        with tempfile.TemporaryDirectory() as directory:
            store = bridge.Store(Path(directory) / 'history.db')
            for number in range(10):
                store.claim(str(number), 'C', 'T', str(number))
                store.update(str(number), 'sent', '답')
            self.assertEqual([x['user'] for x in store.history('C', 'T')],
                             ['4', '5', '6', '7', '8', '9'])

    def test_secrets_not_forwarded(self):
        with patch.dict(os.environ, {'OPENAI_API_KEY': 'secret', 'SLACK_BOT_TOKEN': 'secret',
                                     'CODEX_API_KEY': 'secret', 'OTHER_SECRET': 'secret'}):
            self.assertFalse(any(key in bridge.codex_env() for key in
                                 ['OPENAI_API_KEY', 'SLACK_BOT_TOKEN', 'CODEX_API_KEY', 'OTHER_SECRET']))

    def test_api_auth_rejected(self):
        with patch.object(bridge, 'codex_binary', return_value='codex'), \
             patch.object(bridge.subprocess, 'run', return_value=subprocess.CompletedProcess(
                 [], 0, '', 'Logged in using an API key')):
            with self.assertRaises(RuntimeError):
                bridge.check_auth()

    def test_prompt_is_data_and_no_shell_interpolation(self):
        question = '$(touch /tmp/should-not-exist); `whoami`'
        with patch.object(bridge, 'check_auth'), \
             patch.object(bridge, 'codex_binary', return_value='codex'):
            def fake_run(command, **kwargs):
                self.assertNotIn('shell', kwargs)
                self.assertEqual(kwargs['input'], question)
                self.assertNotIn(question, command)
                self.assertIn('read-only', command)
                self.assertIn('forced_login_method="chatgpt"', command)
                Path(command[command.index('-o') + 1]).write_text('PM 답변')
                return subprocess.CompletedProcess(command, 0, '', '')
            with patch.object(bridge.subprocess, 'run', side_effect=fake_run):
                self.assertEqual(bridge.generate(question), 'PM 답변')

    def test_cli_failure_does_not_leak_stderr(self):
        with patch.object(bridge, 'check_auth'), \
             patch.object(bridge, 'codex_binary', return_value='codex'), \
             patch.object(bridge.subprocess, 'run', return_value=subprocess.CompletedProcess(
                 [], 1, '', 'private-secret')):
            with self.assertRaises(RuntimeError) as error:
                bridge.generate('질문')
            self.assertNotIn('private-secret', str(error.exception))


if __name__ == '__main__':
    unittest.main()
