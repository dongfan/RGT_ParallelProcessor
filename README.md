# RGT ParallelProcessor

## 📋 프로젝트 개요

**RGT ParallelProcessor**는 C++17 STL을 활용하여 멀티스레딩과 함수형 프로그래밍을 구현한 병렬 데이터 처리 라이브러리입니다. 대용량 이미지 데이터 처리를 위한 고성능 병렬 처리 엔진을 제공합니다.

## ✨ 주요 특징

- **템플릿 기반 설계**: 다양한 데이터 타입에 대해 유연한 병렬 처리 지원
- **함수형 프로그래밍**: Lambda 함수와 함수형 접근법을 통한 직관적인 API
- **STL 기반 구현**: C++17 표준 라이브러리만을 사용한 순수 C++ 구현
- **타입 안전성**: `decltype`과 `auto`를 활용한 컴파일 타임 타입 추론
- **성능 최적화**: 청크 기반 작업 분할과 `std::async`를 통한 효율적인 병렬 처리

## 🏗️ 아키텍처

### 핵심 구성요소

```
RGT_ParallelProcessor/
├── include/
│   ├── ParallelProcessor.hpp    # 클래스 선언
│   └── ParallelProcessor.tpp    # 템플릿 구현부
├── src/
│   └── main.cpp                 # 데모 및 성능 측정
├── CMakeLists.txt               # 빌드 설정
└── README.md                    # 프로젝트 문서
```

### ParallelProcessor<T> 클래스

```cpp
template <typename T>
class ParallelProcessor {
public:
    explicit ParallelProcessor(size_t numThreads);
    
    template <typename Func>
    auto parallel_map(const std::vector<T>& data, Func func)
        -> std::vector<decltype(func(std::declval<T>()))>;
        
private:
    size_t numThreads;
};
```

## 🚀 핵심 기능

### 1. parallel_map
- **목적**: 벡터의 모든 요소에 함수를 병렬로 적용
- **특징**: 
  - 입력 타입과 출력 타입이 다를 수 있음 (타입 변환 지원)
  - 컴파일 타임 타입 추론으로 안전성 보장
  - 청크 기반 작업 분할로 효율적인 로드 밸런싱

```cpp
// 예시: int -> int 변환
auto brightened = processor.parallel_map(pixels, [](int pixel) {
    return std::min(255, pixel + 50);
});

// 예시: int -> string 변환
auto strings = processor.parallel_map(pixels, [](int pixel) -> std::string {
    return "pixel_" + std::to_string(pixel);
});
```

### 2. 병렬 처리 알고리즘
- **작업 분할**: 데이터를 스레드 수만큼 청크로 분할
- **동기화**: `std::future`와 `std::async`를 사용한 비동기 처리
- **메모리 효율성**: 결과 벡터 사전 할당으로 메모리 재할당 최소화

## 📊 성능 특성

### 벤치마크 결과
```
Processing 1000000 elements with 4 threads
Sequential time : 953 ms
Parallel time   : 250 ms
Speed up        : 3.812x
```

### 성능 최적화 요소
1. **CPU 집약적 연산**: 삼각함수, 지수함수 등을 통한 충분한 연산량 확보
2. **메모리 지역성**: 순차적 메모리 접근 패턴으로 캐시 효율성 극대화
3. **오버헤드 최소화**: 적절한 청크 크기로 스레드 생성/동기화 비용 최적화

## 🛠️ 빌드 및 실행

### 전제 조건
- **C++17** 호환 컴파일러 (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.12+**
- **pthread** 라이브러리 (Unix 계열)

### 빌드 방법

#### Windows (MinGW/GCC)
```bash
# 직접 컴파일
g++ -std=c++17 -I./include src/main.cpp -o ImageFilterX.exe -pthread -O2

# 또는 CMake 사용
mkdir build && cd build
cmake ..
cmake --build .
```

#### Linux/macOS
```bash
mkdir build && cd build
cmake ..
make -j4
```

### 실행
```bash
./ImageFilterX.exe  # Windows
./main              # Linux/macOS
```

## 💻 사용 예제

### 기본 사용법
```cpp
#include "ParallelProcessor.hpp"

int main() {
    // 1. 프로세서 생성 (4개 스레드)
    ParallelProcessor<int> processor(4);
    
    // 2. 데이터 준비
    std::vector<int> data(1000000);
    std::iota(data.begin(), data.end(), 0);
    
    // 3. 병렬 처리
    auto results = processor.parallel_map(data, [](int x) {
        return x * x;  // 제곱 연산
    });
    
    return 0;
}
```

### 고급 사용 사례

#### 이미지 밝기 조정
```cpp
auto brightened = processor.parallel_map(pixels, [](int pixel) {
    return std::min(255, pixel + 50);
});
```

#### 타입 변환 (int → string)
```cpp
auto labels = processor.parallel_map(pixels, [](int pixel) -> std::string {
    return "pixel_" + std::to_string(pixel);
});
```

#### 수학 연산
```cpp
auto processed = processor.parallel_map(data, [](int x) {
    double result = x;
    for (int i = 0; i < 100; ++i) {
        result = std::sin(result) * std::cos(result);
    }
    return static_cast<int>(result) % 256;
});
```

## 📈 성능 튜닝 가이드

### 최적의 스레드 수
```cpp
// CPU 코어 수에 맞춰 설정
size_t optimal_threads = std::thread::hardware_concurrency();
ParallelProcessor<int> processor(optimal_threads);
```

### 작업 크기 조정
- **작은 작업**: 오버헤드가 클 수 있음 (< 10,000 요소)
- **큰 작업**: 메모리 사용량 증가 (> 10,000,000 요소)
- **권장 범위**: 100,000 ~ 1,000,000 요소

## 🧪 테스트 및 검증

### 정확성 검증
```cpp
// 순차 처리 결과와 병렬 처리 결과 비교
bool verify_results(const std::vector<int>& sequential, 
                   const std::vector<int>& parallel) {
    return std::equal(sequential.begin(), sequential.end(), 
                     parallel.begin());
}
```
