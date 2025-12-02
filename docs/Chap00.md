# Chap 00. GCC & GDB

> **"If you don't know how your compiler works, you don't know how your code works."**
>
> 우리는 코드를 작성하지만, 컴퓨터가 실행하는 것은 기계어다. 그 사이의 간극을 메워주는 **Compiler**와 실행 중인 프로세스의 내장을 들여다보는 **Debugger**를 엔지니어 관점에서 이해한다.

![Tool](https://img.shields.io/badge/Tool-GCC-green)
![Tool](https://img.shields.io/badge/Tool-GDB-red)
![Concept](https://img.shields.io/badge/Concept-ELF%20%2F%20PTrace-blue)

## 1. GCC: Not Just a Compiler
`gcc`는 단순히 C코드를 실행 파일로 바꿔주는 마법 상자가 아니다. 내부적으로 4단계의 파이프라인(Pipeline)을 거치는 **Driver Program**이다.

### 🔍 Deep Dive: The 4 Stages of Build
시스템 엔지니어라면 소스 코드가 실행 파일이 되는 과정을 단계별로 볼 수 있어야 한다.

| 단계 | 역할 | 명령어 옵션 | 결과물 |
| :---: | :--- | :---: | :---: |
| **1. Preprocessing** | `#include`, `#define` 등 매크로 처리, 주석 제거 | `gcc -E` | `.i` (C Source) |
| **2. Compilation** | C 코드를 <b>어셈블리어(Assembly)</b>로 번역 | `gcc -S` | `.s` (Assembly) |
| **3. Assembly** | 어셈블리어를 <b>기계어(Machine Code)</b>로 변환 | `gcc -c` | `.o` (Object File) |
| **4. Linking** | 여러 `.o` 파일과 라이브러리를 합쳐 실행 파일 생성 | `gcc -o` | `a.out` (Executable) |

#### 💡 OS Connection: ELF Format
리눅스에서 생성된 실행 파일(`a.out`)은 **ELF (Executable and Linkable Format)** 라는 표준 규격을 따른다.
* 윈도우의 `.exe` (PE 포맷)와는 구조가 다르다.
* **Header:** 실행 가능한 아키텍처(ARM/x86), 진입점(Entry Point) 주소 정보 포함.
* **Section:** 코드 영역(.text), 데이터 영역(.data, .bss) 등으로 나뉘어 메모리에 로드된다.

> **Tip:** `readelf -h a.out` 명령어로 실행 파일의 헤더 정보를 뜯어볼 수 있다.

---

## 2. GDB: The Surgeon's Knife
`printf` 디버깅은 훌륭하지만, 프로그램이 죽었거나(Crash) 메모리 내부 상태를 정밀하게 확인하려면 **GDB (GNU Debugger)**가 필수다.

### 🔍 Deep Dive: How GDB Works?
GDB는 마법이 아니다. 리눅스 커널이 제공하는 기능을 이용해 다른 프로세스를 제어한다.

#### 💡 OS Connection: `ptrace` System Call
* GDB는 <b>`ptrace`</b>라는 리눅스 시스템 콜을 사용하여 디버깅할 프로세스(Tracee)를 감시한다.
* Breakpoint를 걸면, GDB는 해당 위치의 기계어 코드를 <b>인터럽트 명령어(`INT 3`)</b>로 몰래 바꿔치기한다.
* CPU가 실행하다가 이 명령어를 만나면 **SIGTRAP** 시그널이 발생하고, 운영체제는 실행을 멈춘 뒤 GDB에게 제어권을 넘긴다.

---

## 🧪 Lab: The Hard Way Workflow
실제 자료구조 학습 시 사용할 컴파일 및 디버깅 루틴을 실습한다.

### 3.1 작성 및 컴파일 (--save-temps)
`--save-temps` 옵션을 쓰면 컴파일 중간 파일들(`.i`, `.s`, `.o`)을 지우지 않고 남겨준다. 공부할 때 아주 유용하다.

```bash
$vi test.c
$gcc -save-temps -g -Wall -o test test.c
$ls -l
# test.i (전처리됨), test.s (어셈블리), test.o (기계어), test (실행파일) 확인
```

### 3.2 어셈블리 코드 확인하기
내가 짠 C 코드가 실제로 어떤 어셈블리어로 바뀌었는지 확인한다. 자료구조의 포인터 연산이 실제로 어떻게 동작하는지 이해하는 지름길이다.

```bash
$cat test.s
# mov, push, pop 등의 명령어 확인
```

### 3.3 GDB 실전 명령어 (Cheatsheet)

```bash
$gdb ./test
```

##### 3.3.1 프로세스 시작 및 연결 (Start & Attach)
**상황:** 디버깅을 시작하거나, 이미 돌고 있는 데몬/프로세스에 붙어야 할 때.

| 명령어 | 단축 | 설명 | 사용 상황 |
| :--- | :--- | :--- | :--- |
| `run [args]` | `r` | 프로그램 시작 (인자 전달 가능) | 디버깅 시작할 때 가장 기본 |
| `start` | - | `main()`에서 멈춘 상태로 시작 | `b main` 후 `r` 하기 귀찮을 때 |
| `attach [PID]` | - | 실행 중인 프로세스에 GDB 연결 | 죽지 않은 데몬/서버 디버깅 시 (필수) |
| `detach` | - | 연결된 프로세스를 놔주고 GDB만 종료 | `attach` 후 디버깅 끝났을 때 |
| `kill` | `k` | 디버깅 중인 프로세스 강제 종료 | 프로그램이 먹통일 때 리셋 용도 |
| `quit` | `q` | GDB 종료 | 종료할 때 |

---

##### 3.3.2 중단점 및 감시점 (Breakpoints & Watchpoints)
**상황:** 특정 위치에서 멈추거나, "이 변수 값이 변할 때" 멈추고 싶을 때.

| 명령어 | 단축 | 설명 | 사용 상황 |
| :--- | :--- | :--- | :--- |
| `break [위치]` | `b` | 함수명, 행 번호, 주소에 중단점 설정 | `b main`, `b 15`, `b *0x4005c` |
| `info break` | `i b` | 설정된 모든 중단점 목록 확인 | 내가 어디어디 걸어놨는지 잊었을 때 |
| `delete [번호]` | `d` | 특정 중단점 삭제 (`d`만 치면 전체 삭제) | 필요 없는 중단점 정리 |
| `disable [번호]` | `dis` | 중단점 잠시 끄기 (지우지는 않음) | 루프 안이라 너무 자주 걸릴 때 |
| `enable [번호]` | `en` | 껐던 중단점 다시 켜기 | 다시 확인할 때 |
| `watch [변수]` | - | 변수 값이 '바뀔 때' 멈춤 (쓰기) | "도대체 누가 내 메모리를 덮어쓰지?" 찾을 때 |
| `rwatch [변수]` | - | 변수 값을 '읽을 때' 멈춤 (읽기) | 누가 이 값을 참조하는지 알 때 |

---

##### 3.3.3 실행 흐름 제어 (Flow Control)
**상황:** 코드를 한 줄씩, 혹은 명령어 단위로 실행하며 흐름을 추적할 때.

| 명령어 | 단축 | 설명 | 사용 상황 |
| :--- | :--- | :--- | :--- |
| `continue` | `c` | 다음 중단점까지 쭉 실행 | 현재 위치 분석 끝났을 때 |
| `next` | `n` | 소스 코드 한 줄 실행 (**함수 진입 X**) | 함수 내부 관심 없고 넘어갈 때 |
| `step` | `s` | 소스 코드 한 줄 실행 (**함수 진입 O**) | 함수 내부 동작이 의심될 때 |
| `nexti` | `ni` | 기계어(Assembly) 한 줄 실행 (함수 X) | 소스 코드가 없는 곳, 라이브러리 분석 시 |
| `stepi` | `si` | 기계어(Assembly) 한 줄 실행 (함수 O) | 정밀한 어셈블리 레벨 디버깅 시 |
| `finish` | `fin` | 현재 함수가 리턴할 때까지 실행 | 실수로 함수 들어왔는데 빨리 나가고 싶을 때 |
| `until` | `u` | 루프(Loop)를 빠져나갈 때까지 실행 | `for`문 100번 도는 거 `n` 치기 힘들 때 |

---

##### 3.3.4 데이터 검사 (Data Inspection)
**상황:** 변수 값, 메모리 상태, 레지스터 값을 확인할 때.

| 명령어 | 단축 | 설명 | 사용 상황 |
| :--- | :--- | :--- | :--- |
| `print [변수]` | `p` | 변수 값 출력 (`p/x` : 16진수) | 가장 기본적인 값 확인 |
| `x /[개수][형식] [주소]` | `x` | 메모리 주소의 실제 값 확인 (덤프) | 포인터, 배열, 객체 내부 뜯어볼 때 |
| `display [변수]` | - | 매 스텝마다 변수 값을 자동으로 출력 | 루프 돌면서 변수 변화 관찰할 때 |
| `undisplay` | - | `display` 설정 해제 | 화면이 너무 지저분할 때 |
| `set var [변수]=[값]` | - | 실행 중 변수 값을 강제로 변경 | "이 값이 0이 아니면 버그가 안 날까?" 테스트 |

---

##### 3.3.5 스택 및 정보 (Stack & Info)
**상황:** 프로그램이 죽었을 때(Crash) "어쩌다 여기까지 왔는지" 족보를 캘 때.

| 명령어 | 단축 | 설명 | 사용 상황 |
| :--- | :--- | :--- | :--- |
| `backtrace` | `bt` | 함수 호출 스택(Call Stack) 확인 | 세그폴트(Segfault) 났을 때 필수 1순위 |
| `frame [번호]` | `f` | 특정 스택 프레임으로 이동 | `bt`로 본 상위 함수의 변수를 보고 싶을 때 |
| `info locals` | - | 현재 함수의 지역 변수 전체 출력 | 일일이 `p` 찍기 귀찮을 때 |
| `info args` | - | 현재 함수로 넘어온 인자값 확인 | 함수가 이상한 값을 받았는지 확인할 때 |

---

##### 3.3.6 로우 레벨 & 어셈블리 (Low-Level)
**상황:** C언어 코드만으로는 이해가 안 되거나 최적화 이슈를 볼 때. (시스템 개발자용)

| 명령어 | 단축 | 설명 | 사용 상황 |
| :--- | :--- | :--- | :--- |
| `disassemble` | `disas` | 현재 함수의 어셈블리 코드 출력 | 기계어 레벨에서 로직 확인 |
| `info registers` | `i r` | CPU 레지스터 전체 값 확인 | RAX, RSP 등 레지스터 상태 볼 때 |
| `layout asm` | - | TUI 모드: 어셈블리 창 열기 | 화면 분할해서 보고 싶을 때 |
| `layout src` | - | TUI 모드: 소스 코드 창 열기 | 화면 분할해서 보고 싶을 때 |

---

## 📝 Study Rules

1.  **Look at Assembly:** 가끔은 `cat *.s`를 통해 C언어 한 줄이 CPU에게 어떤 명령을 내리는지 훔쳐본다. (예: `i++`가 실제로는 몇 줄의 명령인가?)
2.  **No more printf:** 자료구조(Linked List 등)가 끊어졌을 때, 코드를 수정해서 `printf`를 넣지 말고 **GDB를 켜서 포인터 주소를 직접 찍어본다.**