#pragma once

#include <string>
#include <vector>

inline void merge(std::vector<std::string> &data, int64_t left, int64_t right,
                  int64_t middle, uint64_t& comparisons) {
    int64_t leftSize = middle - left + 1;
    int64_t rightSize = right - middle;

    std::vector<std::string> leftData(data.begin() + left,
                                      data.begin() + middle + 1);
    std::vector<std::string> rightData(data.begin() + middle + 1,
                                       data.begin() + right + 1);

    int64_t i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        comparisons += std::min(leftData[i].size(), rightData[j].size());
        if (leftData[i] <= rightData[j]) {
            data[k++] = leftData[i++];
        } else {
            data[k++] = rightData[j++];
        }
    }

    while (i < leftSize) {
        data[k++] = leftData[i++];
    }

    while (j < rightSize) {
        data[k++] = rightData[j++];
    }
}

inline void mergeSort(std::vector<std::string> &data, int64_t left,
                      int64_t right, uint64_t& comparisons) {
    if (left < right) {
        auto middle = left + (right - left) / 2;
        mergeSort(data, left, middle, comparisons);
        mergeSort(data, middle + 1, right, comparisons);

        merge(data, left, right, middle, comparisons);
    }
}

inline void MergeSort(std::vector<std::string> &data, uint64_t& comparisons) {
    mergeSort(data, 0, data.size() - 1, comparisons);
}
