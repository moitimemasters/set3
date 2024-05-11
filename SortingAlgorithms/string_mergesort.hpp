#pragma once

#include <string>
#include <vector>

inline auto LcpCompare(const std::string& lhs, const std::string& rhs) {
    int64_t i = 0;
    while (i < lhs.length() && i < rhs.length() && lhs[i] == rhs[i]) {
        i++;
    }
    return i;
}

inline void StringMerge(std::vector<std::string>& data, int64_t left,
                        int64_t mid, int64_t right, uint64_t& comparisons) {
    int64_t i = left, j = mid + 1, k = 0;
    std::vector<std::string> tmp(right - left + 1);

    while (i <= mid && j <= right) {
        int p_i = LcpCompare(data[i], data[j]);
        comparisons += p_i;
        if (p_i >= data[i].length()) {
            tmp[k++] = data[i++];
        } else if (p_i < data[j].length()) {
            tmp[k++] = data[j++];
        } else {
            ++comparisons;
            if (data[i][p_i] <= data[j][p_i]) {
                tmp[k++] = data[i++];
            } else {
                tmp[k++] = data[j++];
            }
        }
    }

    while (i <= mid) {
        tmp[k++] = data[i++];
    }

    while (j <= right) {
        tmp[k++] = data[j++];
    }

    for (int p = 0; p < k; p++) {
        data[left + p] = tmp[p];
    }
}

inline void stringMergeSort(std::vector<std::string>& data, int64_t left,
                            int64_t right, uint64_t& comparisons) {
    if (left >= right) {
        return;
    }

    int middle = left + (right - left) / 2;
    stringMergeSort(data, left, middle, comparisons);
    stringMergeSort(data, middle + 1, right, comparisons);
    StringMerge(data, left, middle, right, comparisons);
}

inline void StringMergeSort(std::vector<std::string>& data,
                            uint64_t& comparisons) {
    stringMergeSort(data, 0, data.size() - 1, comparisons);
}
