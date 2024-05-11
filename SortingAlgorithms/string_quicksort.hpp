#pragma once

#include <string>
#include <vector>

inline void stringQuickSort(std::vector<std::string>& R, int64_t left,
                            int64_t right, int64_t L, uint64_t& comparisons) {
    if (left >= right || L >= R[left].length()) {
        return;
    }

    int64_t i = left, j = right, k = left;
    char pivot = R[(left + right) / 2][L];

    while (k <= j) {
        ++comparisons;
        if (L >= R[k].length() || R[k][L] < pivot) {
            swap(R[i], R[k]);
            i++;
            k++;
        } else if (R[k][L] == pivot) {
            k++;
        } else {
            swap(R[k], R[j]);
            j--;
        }
    }

    stringQuickSort(R, left, i - 1, L, comparisons);
    stringQuickSort(R, i, j, L + 1, comparisons);
    stringQuickSort(R, j + 1, right, L, comparisons);
}

inline void StringQuickSort(std::vector<std::string>& data,
                            uint64_t& comparisons) {
    stringQuickSort(data, 0, data.size() - 1, 0, comparisons);
}
