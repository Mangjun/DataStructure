# 🌳 Data Structures from Scratch: C & Linux

> **"Bad programmers worry about the code. Good programmers worry about data structures and their relationships."**
> 
> — *Linus Torvalds*
>
> 편리한 IDE와 라이브러리 없이, <b>Linux 터미널 환경(Vim, GCC, GDB)</b>에서 C언어로 자료구조를 바닥부터 구현하며 메모리와 시스템의 본질을 학습하는 저장소입니다.

![Language](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-Linux-orange)
![Tools](https://img.shields.io/badge/tools-Vim%20%2F%20GCC%20%2F%20GDB-green)

## 🎯 Goal
자료구조의 이론적 이해를 넘어, <b>"메모리 상에서 데이터가 어떻게 관리되는지"</b>를 직접 확인합니다.
GUI 없는 리눅스 터미널 환경에서 코드를 작성하고 디버깅하며 시스템 엔지니어의 기초 체력을 기르는 것을 목표로 합니다.

* **Implementation:** 라이브러리(`STL`, `Collection`) 없이 순수 C언어와 포인터로 자료구조 직접 구현
* **Environment:** 마우스 없이 키보드만으로 개발하는 리눅스 CLI 환경 숙달 (`vi`)
* **Debugging:** `printf` 디버깅을 넘어, `gdb`를 통해 메모리 주소와 레지스터 상태를 추적

---

## 📚 Current Study: Easy Data Structures in C
기본기가 탄탄한 'C언어로 쉽게 풀어쓴 자료구조'를 메인으로 학습합니다. 단순히 코드를 따라 치는 것이 아니라, 나의 논리로 재구성하여 구현합니다.

* **Book:** C언어로 쉽게 풀어쓴 자료구조 (개정 3판)
* **Link:** [책 구매 링크](https://product.kyobobook.co.kr/detail/S000001076349)

### 🗂️ Chapter Summary
각 챕터별 핵심 구현 내용과 디버깅 과정에서 배운 점, 그리고 **OS와의 연결고리**를 정리합니다.

| Chapter | Topic | Status | Summary |
| :---: | :--- | :---: | :---: |
| **00** | **GCC & GDB** | ✅ | [Chap00](https://github.com/Mangjun/DataStructure/blob/main/docs/Chap00.md) |
| **01** | **자료구조와 알고리즘** | ✅ | [Chap01](https://github.com/Mangjun/DataStructure/blob/main/docs/Chap01.md) |
| **02** | **순환** | ✅ | [Chap02](https://github.com/Mangjun/DataStructure/blob/main/docs/Chap02.md) |
| **03** | **배열, 구조체, 포인터** | 🏃 | []() |
| **04** | **스택** | 🔒 | []() |
| **05** | **큐** | 🔒 | []() |
| **06** | **연결 리스트** | 🔒 | []() |
| **07** | **트리** | 🔒 | []() |
| **08** | **우선순위 큐** | 🔒 | []() |
| **09** | **정렬** | 🔒 | []() |
| **10** | **그래프** | 🔒 | []() |

---

## 🛠️ Development Environment (The Hard Way)
화려한 IDE의 도움을 배제하고, 리눅스 본연의 도구들을 사용하여 개발합니다.

* **OS:** Linux (Ubuntu 22.04 LTS)
* **Editor:** **`vi` (Vim)**
    * 플러그인 최소화, 순수 편집기 기능 숙달
* **Compiler:** **`gcc`**
    * `MakeFile` 작성을 통한 빌드 자동화 이해
    * 컴파일 옵션(`-g`, `-O2`, `-Wall`) 활용
* **Debugger:** **`gdb`**
    * Segmentation Fault 발생 시 Core Dump 분석
    * 포인터가 가리키는 메모리 값 직접 검증

---

## 📝 Study Rules

1.  **No Magic** : 라이브러리 함수 사용을 지양하고, `malloc`, `free`를 사용하여 메모리를 직접 할당하고 해제한다. 메모리 누수(Memory Leak)는 `valgrind` 등으로 철저히 검사한다.
2.  **Raw Coding** : 자동 완성이 없는 `vi` 환경에서 코드를 작성하며 문법과 라이브러리 구조를 머릿속에 각인시킨다.
3.  **Connect to OS (★)** :
    * 단순히 자료구조를 구현하는 것에 그치지 않는다.
    * <b>"이 자료구조가 운영체제(OS) 내부에서는 어디에 쓰이는가?"</b>를 반드시 조사하고 정리한다.