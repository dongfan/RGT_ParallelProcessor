#pragma once

template <typename T>
ParallelProcessor<T>::ParallelProcessor(size_t numThreads)
    : numThreads(numThreads) {}

template <typename T>
template <typename Func>
auto ParallelProcessor<T>::parallel_map(const std::vector<T>& data, Func func)
    -> std::vector<decltype(func(std::declval<T>()))>
{
    using ReturnType = decltype(func(std::declval<T>()));
    std::vector<ReturnType> results(data.size());

    size_t chunkSize = (data.size() + numThreads - 1) / numThreads;
    std::vector<std::future<void>> futures;

    for (size_t t = 0; t < numThreads; ++t) {
        size_t start = t * chunkSize;
        size_t end = std::min(start + chunkSize, data.size());

        if (start >= data.size()) break;

        futures.push_back(std::async(std::launch::async, [&, start, end]() {
            for (size_t i = start; i < end; ++i) {
                results[i] = func(data[i]);
            }
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    return results;
}
