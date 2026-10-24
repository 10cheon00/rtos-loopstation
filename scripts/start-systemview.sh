#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
openocd_bin="${OPENOCD:-/opt/homebrew/bin/openocd}"
# Detect an existing session before building, resetting, or flashing the board.
python3 - <<'PORTCHECK'
import socket
import sys
for host, port in (("0.0.0.0", 19111), ("127.0.0.1", 19021)):
    with socket.socket() as listener:
        listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            listener.bind((host, port))
        except OSError as exc:
            print(f"Cannot start SystemView: TCP {port} is unavailable ({exc}).", file=sys.stderr)
            print(f"Check: lsof -nP -iTCP:{port} -sTCP:LISTEN", file=sys.stderr)
            print("Stop the existing session with Ctrl+C, then retry.", file=sys.stderr)
            sys.exit(1)
PORTCHECK
args=()
case "${1:-}" in
  --flash)
    cmake --preset Debug -DLOOPSTATION_SYSTEMVIEW=ON
    cmake --build --preset Debug
    args=(-c "set FIRMWARE {build/Debug/rtos-loopstation.elf}")
    ;;
  '') ;;
  *) echo "Usage: $0 [--flash]" >&2; exit 2 ;;
esac
"$openocd_bin" "${args[@]}" -f tools/systemview/openocd.cfg &
openocd_pid=$!
bridge_pid=""
cleanup() {
  if [[ -n "$bridge_pid" ]]; then
    kill "$bridge_pid" 2>/dev/null || true
    wait "$bridge_pid" 2>/dev/null || true
  fi
  kill "$openocd_pid" 2>/dev/null || true
  wait "$openocd_pid" 2>/dev/null || true
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
# Flash/erase/verify can take longer than ten seconds.
# Keep the startup allowance separate from the short socket probe timeout.
startup_timeout="${SYSTEMVIEW_STARTUP_TIMEOUT:-120}"
if ! [[ "$startup_timeout" =~ ^[1-9][0-9]*$ ]]; then
  echo "SYSTEMVIEW_STARTUP_TIMEOUT must be a positive integer (seconds)." >&2
  exit 2
fi
startup_deadline=$((SECONDS + startup_timeout))
while (( SECONDS < startup_deadline )); do
  if ! kill -0 "$openocd_pid" 2>/dev/null; then
    status=0
    wait "$openocd_pid" || status=$?
    echo "OpenOCD exited before RTT was ready (status $status)." >&2
    exit 1
  fi
  if python3 -c 'import socket; socket.create_connection(("127.0.0.1",19021),0.1).close()' 2>/dev/null; then
    echo "OpenOCD RTT ready at 127.0.0.1:19021; starting SystemView IP Recorder on TCP 19111."
    python3 tools/systemview/bridge.py &
    bridge_pid=$!
    # End the whole session if either component exits; do not leave an orphan bridge.
    while kill -0 "$openocd_pid" 2>/dev/null && kill -0 "$bridge_pid" 2>/dev/null; do
      sleep 0.2
    done
    if ! kill -0 "$openocd_pid" 2>/dev/null; then
      echo "OpenOCD stopped; shutting down the SystemView bridge." >&2
      exit 1
    fi
    wait "$bridge_pid"
    exit
  fi
  sleep 0.2
done
echo 'OpenOCD RTT startup timed out; check the probe and firmware initialization.' >&2
exit 1
