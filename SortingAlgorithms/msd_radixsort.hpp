#pragma once

#include <string>
#include <vector>

#include "string_quicksort.hpp"

constexpr int64_t ALPHABET_SIZE = 256;

inline void regularMSDRadixSort(std::vector<std::string>& data, int64_t left,
                                int64_t right, int64_t L, uint64_t& comparisons) {
    if (left >= right || L >= data[left].length()) {
        return;
    }

    std::vector<std::string> buckets[ALPHABET_SIZE];

    for (auto i = left; i <= right; i++) {
        ++comparisons;
        auto bucket_index = (L < data[i].length()) ? (int64_t)data[i][L] : 0ll;
        buckets[bucket_index].push_back(data[i]);
    }

    int index = left;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (const std::string& s : buckets[i]) {
            data[index++] = s;
        }
        regularMSDRadixSort(data, left, index - 1, L + 1, comparisons);
        left = index;
    }
}

inline void optimizedMSDRadixSort(std::vector<std::string>& data, int64_t left,
                                  int64_t right, int64_t L, uint64_t& comparisons) {
    if (right - left <= ALPHABET_SIZE) {
        stringQuickSort(data, left, right, L, comparisons);
        return;
    }
    std::vector<std::string> buckets[ALPHABET_SIZE];
    for (auto i = left; i <= right; i++) {
        ++comparisons;
        auto bucket_index = (L < data[i].length()) ? (int64_t)data[i][L] : 0ul;
        buckets[bucket_index].push_back(data[i]);
    }
    int index = left;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (const std::string& s : buckets[i]) {
            data[index++] = s;
        }
        optimizedMSDRadixSort(data, left, index - 1, L + 1, comparisons);
        left = index;
    }
}

inline void RegularMSDRadixSort(std::vector<std::string>& data,
                                uint64_t& comparisons) {
    regularMSDRadixSort(data, 0, data.size() - 1, 0, comparisons);
}

inline void MSDRadixSortWithQuicksort(std::vector<std::string>& data,
                                      uint64_t& comparisons) {
    optimizedMSDRadixSort(data, 0, data.size() - 1, 0, comparisons);
}
