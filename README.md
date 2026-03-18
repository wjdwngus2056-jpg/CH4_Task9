# CH4_Task9

Unreal Engine 기반의 간단한 턴제 숫자야구 프로젝트입니다.

## 개요

- 플레이어는 채팅 입력창을 통해 3자리 숫자를 입력합니다.
- 서버는 입력값을 숫자야구 규칙으로 판정합니다.
- 게임은 턴제로 진행됩니다.
- 현재 턴 플레이어, 남은 시간, 시도 횟수는 HUD에 표시됩니다.
- 승리 시 승자 문구를 잠시 보여준 뒤 게임이 리셋됩니다.

## 주요 클래스

### `ATask9GameModeBase`

게임의 실제 규칙과 턴 진행을 담당합니다.

- 비밀 숫자 생성
- 입력값 검증
- 스트라이크/볼 판정
- 턴 시작/종료/전환
- 승리 판정
- 게임 리셋

관련 파일:

- `Source/Task9/Game/Task9GameModeBase.h`
- `Source/Task9/Game/Task9GameModeBase.cpp`

### `ATask9GameStateBase`

HUD 표시용 상태를 복제합니다.

- `CurrentTurnPlayerName`
- `RemainingTurnTime`
- `MaxTurnTime`
- `CurrentTurnGuessCount`
- `MaxTurnGuessCount`
- `ResultMessage`

관련 파일:

- `Source/Task9/Game/Task9GameStateBase.h`
- `Source/Task9/Game/Task9GameStateBase.cpp`

### `ATask9PlayerController`

로컬 HUD 생성과 채팅 입력 전달을 담당합니다.

- HUD 위젯 생성
- 입력 문자열을 서버로 전달
- 서버가 보낸 채팅/시스템 메시지를 HUD로 전달

관련 파일:

- `Source/Task9/Player/Task9PlayerController.h`
- `Source/Task9/Player/Task9PlayerController.cpp`

### `UTask9HUD`

게임 공용 HUD 위젯의 C++ 부모 클래스입니다.

- 턴 타이머 ProgressBar 갱신
- 현재 턴 플레이어 이름 갱신
- 시도 횟수 갱신
- 승리 문구 표시/숨김 처리
- 내부 채팅 위젯으로 메시지 전달

관련 파일:

- `Source/Task9/UI/Task9HUD.h`
- `Source/Task9/UI/Task9HUD.cpp`

### `UTask9ChatInput`

채팅 입력과 채팅 로그 표시를 담당합니다.

- 입력창 이벤트 처리
- ScrollBox에 채팅 메시지 추가

관련 파일:

- `Source/Task9/UI/Task9ChatInput.h`
- `Source/Task9/UI/Task9ChatInput.cpp`

## 블루프린트 연결

### `WBP_Task9HUD`

부모 클래스를 `Task9HUD`로 설정해야 합니다.

아래 위젯 이름을 C++와 동일하게 맞춰야 합니다.

- `TurnTimer` : `ProgressBar`
- `WinnerTitleText` : `TextBlock`
- `CurrentTurnPlayerNameText` : `TextBlock`
- `GuessCountText` : `TextBlock`
- `ResultMessageText` : `TextBlock`
- `ChatInputWidget` : `WBP_ChatInput`

`WinnerTitleText`는 C++에서 자동으로 `"승리!"`를 넣고, 승리 시에만 보이게 처리합니다.

### `WBP_ChatInput`

부모 클래스를 `Task9ChatInput`으로 설정해야 합니다.

아래 위젯 이름을 맞춰야 합니다.

- `ChatInput` : `EditableTextBox`
- `ChatMessageScrollBox` : `ScrollBox`

## 현재 게임 흐름

1. 게임 시작 시 비밀 숫자를 생성합니다.
2. 플레이어가 접속하면 순서대로 `Player1`, `Player2` 이름을 부여합니다.
3. 첫 번째 플레이어부터 턴을 시작합니다.
4. 현재 턴 플레이어만 숫자 입력이 가능합니다.
5. 유효한 숫자 입력 시 시도 횟수가 증가합니다.
6. 시간 종료 또는 최대 시도 횟수 도달 시 다음 플레이어로 턴이 넘어갑니다.
7. 정답을 맞히면 승리 메시지를 표시합니다.
8. 잠시 후 `Game Reset` 메시지를 채팅에 출력하고 게임을 리셋합니다.

## 참고

- 턴 시간은 `ATask9GameModeBase`의 `TurnDuration` 값으로 조정할 수 있습니다.
- 승리 메시지 대기 시간은 `ResetDelay` 값으로 조정할 수 있습니다.
- 현재 구조는 HUD를 루트로 두고, 채팅 위젯을 HUD 내부 자식으로 포함하는 방식입니다.
