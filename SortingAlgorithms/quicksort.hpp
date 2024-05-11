#pragma once

#include <string>
#include <vector>

inline uint64_t partition(std::vector<std::string> &data, int64_t low,
                          int64_t high, uint64_t &comparisons) {
    auto pivot = data[high];
    int i = low - 1;
    for (auto j = low; j < high; ++j) {
        comparisons += std::min(data[j].length(), pivot.length());
        if (data[j] <= pivot) {
            std::swap(data[++i], data[j]);
        }
    }
    std::swap(data[i + 1], data[high]);
    return i + 1;
}

inline void quickSort(std::vector<std::string> &data, int64_t low, int64_t high,
                      uint64_t &comparisons) {
    if (low < high) {
        auto pivot = partition(data, low, high, comparisons);
        quickSort(data, low, pivot - 1, comparisons);
        quickSort(data, pivot + 1, high, comparisons);
    }
}

inline void QuickSort(std::vector<std::string> &data, uint64_t &comparisons) {
    quickSort(data, 0, data.size() - 1, comparisons);
}
