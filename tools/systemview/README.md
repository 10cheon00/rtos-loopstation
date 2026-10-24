---
title: Windows SystemView LAN 연결
version: 0.1.2
change_history:
  - date: 2026-10-24
    version: 0.1.2
    summary: TCP IP Recorder의 32바이트 HELLO 및 길이 프레임 처리와 검증 결과를 수정함
  - date: 2026-10-24
    version: 0.1.1
    summary: 중복 resume과 서버 대기 시간을 수정하고 실제 RTT 서버 시작 검증 결과를 반영함
  - date: 2026-10-24
    version: 0.1.0
    summary: SystemView 펌웨어 계측과 OpenOCD 중계 실행 및 Windows 접속 절차를 추가함
---

# Windows SystemView LAN 연결

경로: STM32H743 → ST-LINK/SWD → Mac OpenOCD RTT 채널 1 → Python IP Recorder 중계 → Windows SystemView.
보드 자체 Ethernet이나 J-Link Remote Server는 사용하지 않는다.

## Mac에서 실행

ST-LINK USB 및 보드 전원을 연결하고 ST-LINK를 점유하는 다른 디버거를 종료한다.
프로젝트 루트에서 다음 명령으로 Debug 빌드, flash 검증, 서버를 실행한다.

```sh
./scripts/start-systemview.sh --flash
```

이미 이 펌웨어가 기록되어 있으면 `./scripts/start-systemview.sh`로 실행한다.
서버는 터미널에서 유지되며 Ctrl+C로 종료한다. 시작 대기는 기본 120초이며 `SYSTEMVIEW_STARTUP_TIMEOUT` 환경변수로 초 단위 변경할 수 있다.
`SystemView IP Recorder listening on 0.0.0.0:19111` 출력이 서버 시작 완료 기준이다.
보드 기록 후에는 한 번만 reset run을 수행하며 별도 resume을 호출하지 않는다. OpenOCD 경로는 `OPENOCD` 환경변수로 변경할 수 있다.
`open failed`는 ST-LINK를 열지 못했다는 뜻이다. USB 연결, 전원, 다른 디버거 점유 여부를 확인한다.
RTT 발견 실패 시 펌웨어가 SystemView 초기화 지점까지 실행됐는지 확인한다.

| 용도 | 주소/포트 |
| --- | --- |
| Windows SystemView IP Recorder | Mac LAN IP, TCP 19111 |
| OpenOCD RTT 채널 1 | 127.0.0.1:19021 |
| OpenOCD GDB/Telnet/Tcl | 비활성화 |

중계는 0.0.0.0:19111에서 대기한다. macOS 방화벽에서 Python의 로컬 네트워크 수신을 허용한다.
공유기 포트 포워딩은 필요 없다. 두 컴퓨터는 서로 접근 가능한 LAN에 있어야 하며 게스트 Wi-Fi의 클라이언트 격리는 해제되어야 한다.
Mac 주소는 `ipconfig getifaddr en0`으로 확인한다(유선 연결은 해당 인터페이스 사용).
2026-10-24 확인된 Wi-Fi 주소는 `172.30.1.4`이며 DHCP로 바뀔 수 있다.

## Windows에서 연결

1. SEGGER SystemView를 설치하고 실행한다.
2. PowerShell에서 `Test-NetConnection 172.30.1.4 -Port 19111`을 실행해 `TcpTestSucceeded : True`를 확인한다.
3. SystemView의 Recorder Configuration에서 Recorder를 **IP**로 선택한다.
4. Host/IP에 **172.30.1.4**, Port에 **19111**을 입력한다.
5. Start Recording을 누른다. 작업 이름과 실행 타임라인이 수신되는지 확인한다.
6. Stop Recording 후 필요하면 기록 파일을 저장한다.

IP Recorder를 선택해야 한다. J-Link의 IP 연결 설정이나 OpenOCD GDB 포트는 이 연결에 사용하지 않는다.
동시에 한 SystemView만 연결한다. 보드 reset 또는 재기록 후에는 서버도 다시 시작한다.

## 수집 범위와 구현

공식 SEGGER SystemView/RTT 소스의 버전은 ThirdParty/SEGGER의 각 UPSTREAM.txt에 고정했다.
FreeRTOS 10.4 인터페이스를 10.6.2 커널에 연결하고 작업 레지스트리를 32개로 확장했다.
작업 생성/전환/블로킹/준비 상태 및 지원 RTOS API 이벤트와 SysTick ISR을 계측한다.
다른 주변장치 IRQ의 실행 시간은 별도 진입/종료 계측이 필요하다.
타임스탬프는 DWT CYCCNT와 실제 SystemCoreClock을 사용한다.
32 KiB 업 버퍼와 RTT 제어 블록은 MPU 비캐시 SRAM4(0x38000000)에 배치한다.
호스트가 시작 명령을 보내면 기록하며, 이벤트가 생성될 때 다운 채널 명령을 처리한다.
오디오 실시간 동작에는 계측 비용이 추가된다. Overflow 발생 시 이벤트량이나 SWD 전송량을 조정한다.

OpenOCD의 raw RTT 소켓에는 IP Recorder 핸드셰이크가 없으므로 bridge.py가 32바이트 `SEGGER SystemView` HELLO를 교환한다.
Windows에서 오는 명령은 1바이트 길이와 해당 길이의 데이터로 구성되며, 중계는 길이를 제거한 명령만 RTT 다운 채널로 전달한다.
타깃에서 오는 이벤트는 그대로 Windows에 전달한다. UART용 4바이트 SV HELLO는 TCP IP Recorder에 사용하지 않는다.
수집 기능을 제외하려면 `cmake --preset Debug -DLOOPSTATION_SYSTEMVIEW=OFF` 후 빌드한다.

## 검증 상태

Debug 펌웨어 빌드와 링크 성공. SRAM4 사용량 33,952 B / 64 KiB.
로컬 소켓 모의 시험으로 32바이트 분할 IP HELLO, 분할·연속 명령 프레임의 길이 제거, 이벤트 전달과 재접속 검증 통과.
사용자 실행 로그에서 flash 기록과 검증 성공을 확인했다.
시작 명령 수정 후 실제 보드 RTT 제어 블록(0x38008008) 검색과 raw RTT TCP 19021 및 IP Recorder TCP 19111 대기를 확인했다.
Windows SystemView 타임라인 수신은 아직 검증되지 않았다.

참고: [SEGGER SystemView 공식 가이드](https://doc.segger.com/UM08027_SystemView.html),
[OpenOCD RTT 명령](https://openocd.org/doc/html/General-Commands.html#Real-Time-Transfer-_0028RTT_0029).
