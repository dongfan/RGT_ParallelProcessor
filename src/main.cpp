#include <iostream>
#include <numeric>
#include <string>
#include <chrono>
#include <cmath>
#include "ParallelProcessor.hpp"

// ----------------------------
// Sequential 처리 함수
// ----------------------------
template <typename T, typename Func>
long long measure_sequential(const std::vector<T>& data, std::vector<T>& result, Func func) {
    auto start = std::chrono::high_resolution_clock::now();

    result.clear();
    result.reserve(data.size());
    for (const auto& item : data) {
        result.push_back(func(item));
    }

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// ----------------------------
// Parallel 처리 함수
// ----------------------------
template <typename T, typename Func>
long long measure_parallel(ParallelProcessor<T>& processor,
                           const std::vector<T>& data,
                           std::vector<T>& result,
                           Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    result = processor.parallel_map(data, func);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// ----------------------------
// Config 네임스페이스
// ----------------------------
namespace Config {
    constexpr size_t numElements = 1000000;    // 50,000 픽셀 (실행 시간 단축)
    constexpr size_t numThreads  = 4;        // 4 스레드
    constexpr long long targetSequentialMs = 2000; // 순차 처리 예상 시간 약 2초
}

// ----------------------------
// main
// ----------------------------
int main() {
    // 데이터 생성
    std::vector<int> pixelData(Config::numElements);
    std::iota(pixelData.begin(), pixelData.end(), 0);

    ParallelProcessor<int> processor(Config::numThreads);

    // 추가 기능
    // 밝기 조정 필터 (Brightness Filter)
    auto brightenedImage = processor.parallel_map(pixelData, [](int pixel) {
        return std::min(255, pixel + 50);
    });

    // 픽셀 값을 문자열로 변환 (Pixel to String Conversion)
    auto pixelStrings = processor.parallel_map(pixelData, [](int pixel) -> std::string {
        return "pixel_" + std::to_string(pixel);
    });

    // 대비 강화 효과 (Contrast Enhancement)
    auto squaredPixels = processor.parallel_map(pixelData, [](int pixel) {
        return pixel * pixel;
    });

    long long sleepMicroSec = (Config::targetSequentialMs * 1000) / Config::numElements;

    std::vector<int> seqResult, parResult;

    // 복합 수학 필터 (Heavy Compute Filter) - 성능 측정용
    auto heavyComputeFunc = [](int pixel) {
        // 적당한 수학 연산으로 Sequential ~1000ms, Parallel ~250ms 목표
        double result = pixel;
        for (int i = 0; i < 8; ++i) { 
            result = std::sin(result) * std::cos(result) + std::sqrt(std::abs(result));
            result = std::pow(result, 1.05) + pixel * 0.001;  // 지수를 1.1에서 1.05로 줄임
        }
        return static_cast<int>(result) % 256; // 0-255 범위로 제한
    };

    long long seqTime = measure_sequential(pixelData, seqResult, heavyComputeFunc);
    long long parTime = measure_parallel(processor, pixelData, parResult, heavyComputeFunc);
    double speedup = static_cast<double>(seqTime) / parTime;

    // 결과 출력
    
    std::cout << "brightenedImage[0] = " << brightenedImage[0] << "\n";
    std::cout << "brightenedImage[1] = " << brightenedImage[1] << "\n";
    std::cout << "brightenedImage[100] = " << brightenedImage[100 % Config::numElements] << "\n";
    std::cout << "brightenedImage[" << Config::numElements-1 << "] = "
              << brightenedImage[Config::numElements-1] << "\n\n";

    std::cout << "pixelStrings[0] = " << pixelStrings[0] << "\n";
    std::cout << "pixelStrings[1] = " << pixelStrings[1] << "\n";
    std::cout << "pixelStrings[100] = " << pixelStrings[100 % Config::numElements] << "\n\n";

    std::cout << "squaredPixels[0] = " << squaredPixels[0] << "\n";
    std::cout << "squaredPixels[1] = " << squaredPixels[1] << "\n";
    std::cout << "squaredPixels[10] = " << squaredPixels[10 % Config::numElements] << "\n\n";

    std::cout << "Processing " << Config::numElements << " elements with "
              << Config::numThreads << " threads\n";
    std::cout << "Sequential time : " << seqTime << " ms\n";
    std::cout << "Parallel time   : " << parTime << " ms\n";
    std::cout << "Speed up        : " << speedup << "x\n\n";

    return 0;
}
