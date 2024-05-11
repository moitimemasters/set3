#include "StringGenerator.hpp"

#include <algorithm>
#include <cstring>
#include <random>

const char *alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-";

const uint64_t alphabetSize = strlen(alphabet);

StringGenerator::StringGenerator() : rng(std::random_device()()) {}

StringGenerator::StringGenerator(int seed) : rng(seed) {}

std::string StringGenerator::NextString() {
    std::uniform_int_distribution<uint32_t> stringLengthDistribution(
        minStringLength, maxStringLength);
    std::string result;
    result.reserve(stringLengthDistribution(rng));
    for (uint32_t i = 0; i < stringLengthDistribution(rng); ++i) {
        result += alphabet[std::uniform_int_distribution<uint32_t>(
            0, alphabetSize - 1)(rng)];
    }
    return result;
}

std::vector<std::string> StringGenerator::NextArrayOfStrings(uint64_t length) {
    std::vector<std::string> result;
    result.reserve(length);
    for (uint32_t i = 0; i < length; ++i) {
        result.push_back(NextString());
    }
    return result;
}

std::vector<std::vector<std::string>>
StringGenerator::NextFullyRandomTestCase() {
    std::vector<std::vector<std::string>> result;
    result.reserve((maxArrayLength - minArrayLength) / arrayLengthStep + 1);
    for (auto i = minArrayLength; i <= maxArrayLength; i += arrayLengthStep) {
        result.push_back(NextArrayOfStrings(i));
    }
    return result;
}

std::vector<std::vector<std::string>>
StringGenerator::NextReverseSortedTestCase() {
    auto result = NextFullyRandomTestCase();
    for (auto &array : result) {
        std::sort(array.begin(), array.end());
        std::reverse(array.begin(), array.end());
    }
    return result;
}

std::vector<std::vector<std::string>>
StringGenerator::NextAlmostSortedTestCase() {
    auto result = NextFullyRandomTestCase();
    for (auto &array : result) {
        std::sort(array.begin(), array.end());
        for (uint32_t i = 0; i < std::max(array.size() / 10, 15ul); ++i) {
            std::swap(array[std::uniform_int_distribution<uint32_t>(
                          0, array.size() - 1)(rng)],
                      array[std::uniform_int_distribution<uint32_t>(
                          0, array.size() - 1)(rng)]);
        }
    }
    return result;
}
