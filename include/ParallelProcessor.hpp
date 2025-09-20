#ifndef PARALLEL_PROCESSOR_HPP
#define PARALLEL_PROCESSOR_HPP

#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <functional>

// 템플릿 클래스 선언
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

#include "ParallelProcessor.tpp" // 구현부 포함

#endif // PARALLEL_PROCESSOR_HPP