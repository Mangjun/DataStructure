# Chap 03. 배열, 구조체, 그리고 포인터

> **"Pointers are the raw nerve endings of the computer."**
>
> 배열과 구조체는 데이터를 담는 그릇이 아니라, 메모리 상에 데이터가 <b>배치(Layout)</b>되는 규칙이다. 우리는 컴파일러가 숨겨놓은 패딩(Padding)과 캐시 라인(Cache Line)의 비밀을 파헤친다.

![Topic](https://img.shields.io/badge/Topic-Memory%20Layout-blue)
![Architecture](https://img.shields.io/badge/Arch-Alignment%20%26%20Padding-orange)
![Performance](https://img.shields.io/badge/Perf-False%20Sharing-red)

## 1. Array: King of Performance (Spatial Locality)
자료구조 책에서는 배열을 "연속된 메모리 공간"이라고 정의한다. 시스템 엔지니어는 이를 <b>"캐시 히트(Cache Hit)를 보장하는 구조"</b>라고 읽어야 한다.

### 🔍 Deep Dive: Why Array is fast?
CPU는 메모리에서 데이터를 가져올 때 1바이트씩 가져오지 않고, **Cache Line (보통 64Bytes)** 단위로 덩어리째 퍼온다.

* **Array:** 데이터가 다닥다닥 붙어 있다. `arr[0]`을 읽을 때 `arr[1]`, `arr[2]`... 도 같이 캐시에 올라온다. (Spatial Locality)
* **Linked List:** 데이터가 힙 메모리 여기저기에 흩어져 있다. 노드를 건너갈 때마다 캐시 미스(Cache Miss)가 발생하여 CPU가 멈칫한다.

> **💡 Engineering Note (Check Cache)**
> 리눅스 커널이 많은 내부 자료구조에서 연결 리스트보다 <b>배열 기반의 구조(Radix Tree, XArray 등)</b>를 선호하는 이유가 바로 이 캐시 효율성 때문이다. 따라서 성능이 절대적으로 중요한 구간(Hot path)에서는 연결 리스트보다 배열 사용을 먼저 고려해야 한다.

---

## 2. Structure: The Hidden Memory (Alignment & Padding)
`struct`를 정의할 때, 멤버 변수들의 크기 합이 실제 구조체 크기와 다를 수 있다. **Padding(패딩)** 때문이다.

### 🔍 Deep Dive: Memory Alignment
CPU는 메모리에 접근할 때 워드(Word, 4byte or 8byte) 단위로 읽는 것을 좋아한다.

    struct Data {
        char c;   // 1 byte
        int i;    // 4 bytes
    };

위 구조체의 `sizeof(struct Data)`는 5가 아니라 **8**이다.
* CPU가 `int`에 접근할 때 4의 배수 주소에서 읽기 위해, `char` 뒤에 <b>3바이트의 패딩(빈 공간)</b>을 채워넣기 때문이다.

> **💡 Best Practice (Mind the Gap)**
> 구조체를 설계할 때 멤버 변수의 선언 순서만 바꿔도 메모리를 절약할 수 있다. 패딩을 최소화하려면 <b>크기가 큰 자료형부터 내림차순(double → int → char)</b>으로 선언하는 습관을 들이자.

---

## 3. False Sharing: The Silent Performance Killer
구조체의 패딩이 단순히 메모리 정렬을 맞추는 것을 넘어, **멀티코어 성능 문제의 핵심 해결책**이 되는 경우다.

### 🔍 Deep Dive: Cache Line Ping-Pong
서로 다른 스레드가 서로 다른 변수(`x`, `y`)를 수정하는데도 시스템 성능이 급격히 떨어지는 현상이다.
* **원인:** 변수 `x`와 `y`가 우연히 **같은 64Byte 캐시 라인** 안에 위치함.
* **현상:** Core 1이 `x`를 수정하면, 하드웨어는 해당 캐시 라인 전체를 "수정됨(Dirty)" 처리하고 Core 2의 캐시를 **강제로 무효화(Invalidate)** 시킨다. Core 2가 `y`를 쓰려 할 때 다시 메모리에서 읽어와야 한다. (무한 반복)

### 🛠 Solution: Padding for Isolation
의미 없는 데이터를 채워 넣어(Padding), 두 변수가 물리적으로 다른 캐시 라인에 위치하게 강제한다.

```C
    // ❌ Bad: x와 y가 붙어 있어 같은 캐시 라인 공유 가능성 높음
    struct Bad {
        long x;
        long y;
    };

    // ✅ Good: GCC 속성을 이용해 64바이트 경계로 강제 정렬
    struct Good {
        long x;
    } __attribute__((aligned(64)));
```

> **💡 OS Connection**
> 리눅스 커널 소스 코드(`include/linux/cache.h`)를 보면 `____cacheline_aligned` 라는 매크로가 존재한다. 스핀락(Spinlock)이나 전역 카운터처럼 여러 코어가 동시에 접근하는 핵심 데이터 구조는 반드시 이 매크로를 사용하여 False Sharing을 방지한다.

---

## 4. Pointer: The Virtual Address
포인터는 물리 메모리(RAM) 주소가 아니다. 운영체제가 프로세스에게 환상(Illusion)을 보여주는 <b>가상 주소(Virtual Address)</b>다.

### 🔍 Deep Dive: Pointer Arithmetic
포인터 덧셈(`ptr + 1`)은 주소값에 1을 더하는 게 아니라, `sizeof(type)`만큼 더하는 것이다.
* `int *ptr` 에서 `ptr + 1` → 실제 주소는 **+4 bytes**.
* 이 원리를 모르면 배열 인덱싱을 수동으로 구현할 때 엉뚱한 메모리를 찌르게 된다.

> **💡 Engineering Note (Pointer is Power)**
> 단순히 주소를 저장하는 것을 넘어, 현재 이 포인터가 가리키는 곳이 **Stack(지역변수), Heap(동적할당), Data(전역변수)** 영역 중 어디인지 구분할 수 있어야 한다. 영역에 따라 변수의 수명 주기(Lifecycle)가 완전히 다르기 때문이다.