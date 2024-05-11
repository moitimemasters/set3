#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <vector>

class StringGenerator {
   public:
    StringGenerator();
    StringGenerator(int seed);
    std::string NextString();
    std::vector<std::string> NextArrayOfStrings(uint64_t length);
    std::vector<std::vector<std::string>> NextFullyRandomTestCase();
    std::vector<std::vector<std::string>> NextReverseSortedTestCase();
    std::vector<std::vector<std::string>> NextAlmostSortedTestCase();

   private:
    std::mt19937 rng;
    const uint64_t minStringLength = 10;
    const uint64_t maxStringLength = 200;
    const uint64_t minArrayLength = 100;
    const uint64_t maxArrayLength = 3000;
    const uint64_t arrayLengthStep = 100;
};
