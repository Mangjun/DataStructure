# Chap 01. 자료구조와 알고리즘 성능 분석

> **"Latency is the new outage."**
>
> 시스템 프로그래밍에서 시간 복잡도는 '빠름'의 척도이며, 공간 복잡도는 시스템의 '안정성(Stability)'을 결정하는 생존의 문제다.

![Topic](https://img.shields.io/badge/Topic-Time%20%26%20Space%20Complexity-blue)
![OS Connection](https://img.shields.io/badge/Connection-Scheduler%20%2F%20Kernel%20Stack-orange)

## 1. Time Complexity (시간 복잡도)
단순히 알고리즘이 "빠르다/느리다"를 넘어, **입력값(n)이 폭발적으로 늘어났을 때 시스템이 멈추지 않고 반응할 수 있는가?** 를 판단하는 기준이다.

### 🔍 Deep Dive: Why O(n) is dangerous in OS?
일반 애플리케이션 개발에서는 데이터가 적으면 $O(n)$과 $O(1)$의 차이를 느끼기 힘들다. 하지만 운영체제 레벨에서는 치명적이다.

#### 💡 OS Connection: Linux Scheduler Evolution
리눅스 커널 스케줄러(Scheduler)의 역사는 시간 복잡도와의 싸움이었다.

1.  **초기 스케줄러 ($O(n)$)**: 실행 대기 중인 모든 프로세스를 리스트에서 순차적으로 탐색하여 가장 우선순위가 높은 것을 찾음. 프로세스가 많아질수록 시스템이 급격히 느려짐.
2.  **O(1) Scheduler**: 프로세스 개수와 상관없이 **상수 시간($O(1)$)** 안에 다음 프로세스를 선택하도록 자료구조(Bitmap, Queue)를 개선함.
3.  **CFS (Completely Fair Scheduler)**: 현재 리눅스의 표준. <b>레드-블랙 트리(Red-Black Tree)</b>를 사용하여 $O(\log n)$의 효율성과 공정성(Fairness)을 동시에 잡음.

> **Conclusion:**
> 시스템 콜이나 인터럽트 핸들러를 작성할 때, 입력 크기에 따라 실행 시간이 선형적으로 증가하는 $O(n)$ 로직은 시스템 전체의 Latency(지연)를 유발할 수 있으므로 극도로 주의해야 한다.

---

## 2. Space Complexity (공간 복잡도)
메모리를 얼마나 적게 쓰는가의 문제를 넘어, **제한된 자원 안에서 시스템이 뻗지 않게 관리할 수 있는가?** 의 문제다.

### 🔍 Deep Dive: The Kernel Stack Limit
현대의 PC는 수십 GB의 RAM을 가지지만, <b>커널 모드(Kernel Mode)</b>에서의 메모리는 매우 제한적이다.

#### 💡 OS Connection: Kernel Stack Overflow
* **User Space**: 스택 크기가 유동적이며 부족하면 OS가 늘려줄 수 있음 (보통 8MB~).
* **Kernel Space**: 각 프로세스(스레드)마다 할당된 커널 스택은 고정되어 있으며 매우 작다. (아키텍처에 따라 **4KB ~ 16KB** 불과).

만약 커널 내부 함수에서 <b>재귀 호출(Recursion)</b>을 깊게 사용하거나, 덩치 큰 지역 변수(Large Local Variables)를 선언하면 스택 영역을 벗어나게 된다. 이는 곧 <b>Kernel Panic (시스템 붕괴)</b>으로 이어진다.

> **Conclusion:**
> 시스템 프로그래밍(특히 커널)에서는 공간 복잡도가 높은 알고리즘, 특히 '재귀(Recursion)' 사용을 지양하고 반복문(Iteration)으로 구현해야 한다.

---

## 🧪 Lab: Measuring Performance on Linux
이론적인 Big-O 표기법을 실제 리눅스 환경에서 체감해본다.

### `time` 커맨드 활용
리눅스의 `time` 명령어를 통해 프로그램의 실제 실행 시간을 측정한다.

```bash
$gcc -o test_search search_algo.c
$time ./test_search
```