# Chap 02. 순환

> **"To iterate is human, to recurse divine... but not in the Kernel."**
>
> 재귀는 알고리즘을 우아하게 표현하지만, 시스템 소프트웨어에서는 **Stack Overflow**라는 시한폭탄이 될 수 있다.

![Topic](https://img.shields.io/badge/Topic-Recursion%20vs%20Iteration-blue)
![OS Connection](https://img.shields.io/badge/Connection-Kernel%20Stack%20%26%20Context%20Switch-red)

## 1. Recursion: The Double-Edged Sword
순환(재귀)은 단순히 함수가 자신을 호출하는 문법이 아니라, **"거대한 문제를 내가 해결할 수 있는 가장 작은 단위(Base Case)가 될 때까지 쪼개는"**(<b>분할정복법</b>: Divide and Conquer) 문제 해결 전략이다. 팩토리얼, 피보나치 수열, 하노이 탑 등을 코드로 옮길 때 수학적 수식을 그대로 표현할 수 있어 가독성이 좋다.

하지만 <b>"함수 호출(Function Call)"</b>은 공짜가 아니다. 하드웨어 관점에서 함수 호출은 **비용이 비싼 작업**이다.

---

## 2. Deep Dive: What happens in Memory?
우리가 C언어에서 `func()`를 호출할 때, CPU와 메모리에서는 다음과 같은 **오버헤드**가 발생한다.

### 2.1 Stack Frame (스택 프레임)의 생성
함수가 호출될 때마다 메모리의 **스택(Stack)** 영역에 새로운 블록(프레임)이 쌓인다.
* **Return Address:** 함수 종료 후 돌아갈 코드의 주소
* **Saved Registers:** 이전 함수가 쓰던 레지스터 값 백업 (`RBP` 등)
* **Local Variables:** 함수 내에서 선언된 지역 변수
* **Arguments:** 함수로 전달된 인자값

### 2.2 Context Switching within Process
함수 호출이 깊어질수록 CPU는 메모리(Stack)에 데이터를 쓰고 읽는 작업(Push/Pop)을 반복해야 한다. 이는 단순히 반복문(Loop)을 돌 때 레지스터만 사용하는 것에 비해 훨씬 느리다.

---

## 3. OS Connection: The Kernel Stack Hazard
알고리즘 문제 풀이(PS)에서는 "재귀 깊이가 100,000을 넘지 않게 하라" 정도의 제약만 있지만, **운영체제 커널 개발**에서는 이야기가 다르다.

### 3.1 Kernel Stack Size
리눅스 커널은 각 스레드마다 별도의 <b>커널 스택(Kernel Stack)</b>을 할당한다. 이 크기는 매우 작고 고정되어 있다.
* **x86_64 기준:** 보통 **16KB** (페이지 4개 크기) 혹은 설정에 따라 **8KB**.
* **User Space Stack:** 기본 **8MB** (부족하면 OS가 자동으로 늘려줌, Page Fault).

### 3.2 The Danger of Recursion in Kernel
만약 커널 코드(시스템 콜, 디바이스 드라이버) 내에서 깊은 재귀 호출을 사용하면 어떻게 될까?
1.  16KB의 작은 스택 공간이 순식간에 가득 찬다.
2.  스택 포인터(`SP`)가 할당된 메모리 영역을 벗어난다.
3.  중요한 커널 데이터 구조(`thread_info` 등)를 덮어쓰거나(Corruption), 접근 불가능한 메모리를 건드린다.
4.  **Kernel Panic (Oops)** 발생 → 시스템 셧다운.

> **Retrospective:**
> 리눅스 커널 코딩 컨벤션에서는 재귀 호출을 **극도로 지양**한다. 꼭 써야 한다면 깊이(Depth)를 엄격하게 제한하거나, <b>반복문(Iteration)</b>으로 재작성해야 한다.