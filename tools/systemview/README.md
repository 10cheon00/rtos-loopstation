---
title: Windows SystemView LAN 연결
version: 0.2.0
change_history:
  - date: 2026-10-25
    version: 0.2.0
    summary: RTT 수집과 송신을 메모리 큐 및 디스크 임시 저장으로 분리하고 운용 및 한계를 명시함
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

## 수집과 송신의 분리

RTT 수집과 Windows 송신은 독립 비동기 작업으로 실행한다. 수집 작업은 Windows 소켓의 `drain()`을 기다리지 않고 FIFO에 데이터를 저장하며, 송신 작업이 FIFO에서 꺼내 순서대로 전달한다. 같은 프로세스에서도 네트워크 대기는 이벤트 루프에 제어를 반환하므로 수집을 막지 않는다. 임시 파일 I/O는 별도 작업 스레드에서 실행한다.

FIFO 데이터는 기본 256 MiB까지 Mac 메모리에 저장한다. 한도 초과 데이터는 로컬 임시 파일에 저장하고, 메모리 데이터와 동일한 FIFO 순서로 읽어 송신한다. 가득 찬 메모리 큐에서 수집을 멈추거나 임의 바이트를 버리지 않는다. 메모리 한도는 큐의 데이터 기준이며 Python 관리 정보, 소켓 및 전송 중 청크는 별도다.

```sh
# 메모리 큐를 512 MiB로 설정하고 서버 실행 (펌웨어 재기록 불필요)
SYSTEMVIEW_BUFFER_MIB=512 ./scripts/start-systemview.sh

# 임시 저장 위치를 변경하려면 미리 존재하는 로컬 디렉터리 지정
SYSTEMVIEW_BUFFER_MIB=512 SYSTEMVIEW_SPOOL_DIR=/private/tmp ./scripts/start-systemview.sh
```

메모리 사용은 미리 전부 할당하지 않고 대기 데이터에 따라 증가한다. `Capture RAM budget reached`는 디스크 저장으로 전환했다는 알림이며 수집 중단 오류가 아니다. 종료 시 최대 대기 바이트, 디스크 저장 누계, 미송신 대기 바이트를 출력한다.

임시 파일은 연결 세션 종료 시 삭제되며 복구용 녹화 파일이 아니다. 디스크에 넘긴 공간은 세션 종료까지 유지되므로 긴 정체에는 충분한 로컬 저장 공간이 필요하다. 디스크 부족·I/O 오류는 로그와 함께 해당 연결을 종료한다. 무한한 정체를 유한 메모리와 디스크로 무손실 처리할 수는 없다. OpenOCD/SWD 자체의 수집 속도 부족이나 타깃 RTT 버퍼 오버플로는 이 호스트 큐로 해결되지 않는다.

RTT의 정상 EOF 시 대기 데이터를 모두 전송한 뒤 연결을 닫는다. Windows 연결 끊김, 서버 종료, Ctrl+C에서는 해당 세션의 수집·송신을 취소하고 남은 큐를 폐기한다. 연결 복구 시 이전 세션을 자동 재생하지 않는다. 변경한 코드를 적용하려면 기존 서버를 Ctrl+C로 종료하고 다시 실행한다.

## 검증 상태

Debug 펌웨어 빌드와 링크 성공. SRAM4 사용량 33,952 B / 64 KiB.
로컬 소켓 모의 시험으로 32바이트 분할 IP HELLO, 분할·연속 명령 프레임의 길이 제거, 이벤트 전달과 재접속 검증 통과.
Windows 송신을 의도적으로 정체시킨 시험에서 RTT 수집 완료, RAM 한도 유지, 디스크 넘김, FIFO 무손실 전송, RTT EOF 후 큐 전송 완료와 취소 처리를 검증했다. 실제 LAN 장시간 부하 시험은 아직 수행하지 않았다.
사용자 실행 로그에서 flash 기록과 검증 성공을 확인했다.
시작 명령 수정 후 실제 보드 RTT 제어 블록(0x38008008) 검색과 raw RTT TCP 19021 및 IP Recorder TCP 19111 대기를 확인했다.
Windows SystemView 타임라인 수신은 아직 검증되지 않았다.

참고: [SEGGER SystemView 공식 가이드](https://doc.segger.com/UM08027_SystemView.html),
[OpenOCD RTT 명령](https://openocd.org/doc/html/General-Commands.html#Real-Time-Transfer-_0028RTT_0029).
