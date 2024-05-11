#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "SortingAlgorithms/mergesort.hpp"
#include "SortingAlgorithms/msd_radixsort.hpp"
#include "SortingAlgorithms/quicksort.hpp"
#include "SortingAlgorithms/string_mergesort.hpp"
#include "SortingAlgorithms/string_quicksort.hpp"
#include "StringGenerator/StringGenerator.hpp"

void GenerateCommand(std::optional<int> seed,
                     std::optional<std::string> outputFile) {
    std::vector<std::vector<std::string>> regular, reverseSorted, almostSorted;
    std::cout << "GEN COMMAND\n";
    StringGenerator* sg = nullptr;
    if (seed.has_value()) {
        sg = new StringGenerator{seed.value()};
    } else {
        sg = new StringGenerator{};
    }

    regular = sg->NextFullyRandomTestCase();
    reverseSorted = sg->NextReverseSortedTestCase();
    almostSorted = sg->NextAlmostSortedTestCase();

    std::ofstream out;
    if (outputFile.has_value()) {
        out.open(outputFile.value());
    } else {
        out.open("test_cases.txt");
    }

    out << "/ regular\n";
    for (auto& v : regular) {
        out << "/ " << v.size() << ":\n";
        for (auto& s : v) {
            out << s << "\n";
        }
    }

    out << "/ reverse sorted\n";
    for (auto& v : reverseSorted) {
        out << "/ " << v.size() << ":\n";
        for (auto& s : v) {
            out << s << "\n";
        }
    }

    out << "/ almost sorted\n";
    for (auto& v : almostSorted) {
        out << "/ " << v.size() << ":\n";
        for (auto& s : v) {
            out << s << "\n";
        }
    }
}

struct TestCase {
    std::string name;
    std::vector<std::vector<std::string>> arrays;
};

struct TestResult {
    std::string name;
    std::vector<std::pair<size_t, std::vector<std::pair<uint64_t, uint64_t>>>>
        results;
};

typedef void (*SortingAlgorithm)(std::vector<std::string>&, uint64_t&);

SortingAlgorithm GetSortingAlgorithm(const std::string& algorithm) {
    if (algorithm == "qs") {
        return QuickSort;
    } else if (algorithm == "ms") {
        return MergeSort;
    } else if (algorithm == "sqs") {
        return StringQuickSort;
    } else if (algorithm == "sms") {
        return StringMergeSort;
    } else if (algorithm == "msd") {
        return RegularMSDRadixSort;
    } else if (algorithm == "mqs") {
        return MSDRadixSortWithQuicksort;
    }
    throw std::invalid_argument("Unknown sorting algorithm: " + algorithm);
}

std::vector<std::pair<uint64_t, uint64_t>> MeasureSortingTime(
    const SortingAlgorithm sort_func, std::vector<std::string>& test_case,
    uint32_t tries) {
    std::vector<std::pair<uint64_t, uint64_t>> times;
    std::vector<std::string> test_case_copy(test_case);
    uint64_t comparisons;
    for (uint32_t i = 0; i < tries; i++) {
        comparisons = 0;
        auto start = std::chrono::high_resolution_clock::now();
        sort_func(((i == tries - 1) ? test_case : test_case_copy), comparisons);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        times.emplace_back(duration.count(), comparisons);
        test_case_copy = test_case;
    }
    return times;
}

std::vector<TestCase> ParseTestCases(const std::string& inputFile) {
    std::ifstream input_file(inputFile);
    if (!input_file) {
        throw std::runtime_error("Failed to open input file: " + inputFile);
    }

    std::vector<TestCase> test_cases;
    std::string line;
    TestCase current_test_case;

    while (std::getline(input_file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line[0] == '/') {
            if (!current_test_case.arrays.empty()) {
                test_cases.push_back(current_test_case);
                current_test_case.arrays.clear();
            }
            if (line.back() == ':') {
                current_test_case.arrays.push_back({});
            } else {
                current_test_case.name = line.substr(2);
            }
        } else {
            current_test_case.arrays.back().push_back(line);
        }
    }

    if (!current_test_case.arrays.empty()) {
        test_cases.push_back(current_test_case);
    }

    return test_cases;
}

std::vector<TestResult> RunTestCases(std::vector<TestCase>& testCases,
                                     SortingAlgorithm sortFunc,
                                     uint32_t tries) {
    std::vector<TestResult> test_results;

    for (auto& testCase : testCases) {
        TestResult testResult;
        testResult.name = testCase.name;

        for (auto& testArray : testCase.arrays) {
            auto times = MeasureSortingTime(sortFunc, testArray, tries);
            testResult.results.emplace_back(testArray.size(), times);
        }

        test_results.push_back(testResult);
    }

    return test_results;
}

void WriteTestResults(const std::vector<TestResult>& testResults,
                      const std::vector<TestCase>& testCases,
                      const std::string& outputFile) {
    std::ofstream output(outputFile);
    if (!output) {
        throw std::runtime_error("Failed to open output file: " + outputFile);
    }

    for (const auto& testResult : testResults) {
        output << "/ " << testResult.name << "\n";
        for (const auto& arrayResult : testResult.results) {
            output << "/ " << arrayResult.first << ":\n";
            for (const auto& [time, comparisons] : arrayResult.second) {
                output << time << " " << comparisons << "\n";
            }
            output << "\n";
        }
    }

    for (const auto& testCase : testCases) {
        output << "/ " << testCase.name << "\n";
        for (const auto& array : testCase.arrays) {
            output << "/ " << array.size() << ":\n";
            for (const auto& str : array) {
                output << str << "\n";
            }
        }
    }
}

void SortCommand(const std::string& algorithm, uint32_t tries,
                 const std::string& inputFile,
                 std::optional<std::string> outputFile) {
    SortingAlgorithm sortFunc = GetSortingAlgorithm(algorithm);
    std::vector<TestCase> testCases = ParseTestCases(inputFile);
    std::vector<TestResult> testResults =
        RunTestCases(testCases, sortFunc, tries);

    std::string outputFilePath = outputFile.value_or("sorted_test_cases.txt");
    WriteTestResults(testResults, testCases, outputFilePath);
}

int main(int argc, char* argv[]) {
    const char* generateHelpMessage =
        "Usage of command gen: %s gen [options]\n"
        "Options:\n"
        "  -h, --help\t\t\tShow this help message\n"
        "  -s, --seed\t\t\tSeed for generating test cases\n"
        "  -o, --output\t\t\tOutput file\n";

    const char* sortHelpMessage =
        "Usage of command sort: %s sort [options]\n"
        "Options:\n"
        "  -h, --help\t\t\tShow this help message\n"
        "  -a, --algorithm\t\tSelect sorting algorithm\n"
        "  -n, --number\t\t\tNumber of tries\n"
        "  -i, --input\t\t\tInput file\n"
        "  -o, --output\t\t\tOutput file\n";

    const char* entryHelpMessage =
        "Usage of %s: %s [command] [options]\n"
        "Commands:\n"
        "  gen\t\tGenerate test data\n"
        "  sort\t\tSort data\n"
        "Options:\n"
        "  -h, --help\t\tShow this help message\n"
        "Algorithms:\n"
        "  Quicksort\t\t\t - qs\n"
        "  Mergesort\t\t\t - ms\n"
        "  String Quicksort\t\t - sqs\n"
        "  String Mergesort\t\t - sms\n"
        "  MSD Radixsort (w/o qs)\t - msd\n"
        "  MSD Radixsort (w/ qs)\t\t - mqs\n";

    if (argc < 2) {
        printf(entryHelpMessage, argv[0], argv[0]);
        return 1;
    }

    std::string command(argv[1]);

    if (command == "gen") {
        std::optional<int> seed;
        std::optional<std::string> outputFile;

        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printf(generateHelpMessage, argv[0]);
                return 0;
            } else if (strcmp(argv[i], "-s") == 0 ||
                       strcmp(argv[i], "--seed") == 0) {
                if (i + 1 < argc) {
                    seed = std::stoi(argv[i + 1]);
                    ++i;
                } else {
                    std::cout << "Error: Missing value for --seed option."
                              << std::endl;
                    return 1;
                }
            } else if (strcmp(argv[i], "-o") == 0 ||
                       strcmp(argv[i], "--output") == 0) {
                if (i + 1 < argc) {
                    outputFile = argv[i + 1];
                    ++i;
                } else {
                    std::cout << "Error: Missing value for --output option."
                              << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Error: Unknown option " << argv[i] << std::endl;
                return 1;
            }
        }

        GenerateCommand(seed, outputFile);
    } else if (command == "sort") {
        std::string algorithm;
        uint32_t tries = 0;
        std::string inputFile;
        std::optional<std::string> outputFile;

        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printf(sortHelpMessage, argv[0]);
                return 0;
            } else if (strcmp(argv[i], "-a") == 0 ||
                       strcmp(argv[i], "--algorithm") == 0) {
                if (i + 1 < argc) {
                    algorithm = argv[i + 1];
                    // check if algorithm is correct
                    if (algorithm != "qs" && algorithm != "ms" &&
                        algorithm != "sqs" && algorithm != "sms" &&
                        algorithm != "msd" && algorithm != "mqs") {
                        std::cout << "Error: Unknown algorithm " << algorithm
                                  << std::endl;
                        return 1;
                    }
                    ++i;
                } else {
                    std::cout << "Error: Missing value for --algorithm option."
                              << std::endl;
                    return 1;
                }
            } else if (strcmp(argv[i], "-n") == 0 ||
                       strcmp(argv[i], "--number") == 0) {
                if (i + 1 < argc) {
                    tries = std::stoul(argv[i + 1]);
                    ++i;
                } else {
                    std::cout << "Error: Missing value for --number option."
                              << std::endl;
                    return 1;
                }
            } else if (strcmp(argv[i], "-i") == 0 ||
                       strcmp(argv[i], "--input") == 0) {
                if (i + 1 < argc) {
                    inputFile = argv[i + 1];
                    ++i;
                } else {
                    std::cout << "Error: Missing value for --input option."
                              << std::endl;
                    return 1;
                }
            } else if (strcmp(argv[i], "-o") == 0 ||
                       strcmp(argv[i], "--output") == 0) {
                if (i + 1 < argc) {
                    outputFile = argv[i + 1];
                    ++i;
                } else {
                    std::cout << "Error: Missing value for --output option."
                              << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Error: Unknown option " << argv[i] << std::endl;
                return 1;
            }
        }

        if (algorithm.empty() || tries == 0 || tries > 100 ||
            inputFile.empty()) {
            std::cout << "Error: Missing required options for sort command."
                      << std::endl;
            printf(sortHelpMessage, argv[0]);
            return 1;
        }
        std::cout << algorithm << " " << tries << " " << inputFile << " "
                  << std::endl;

        SortCommand(algorithm, tries, inputFile, outputFile);
    } else {
        std::cout << "Error: Unknown command " << command << std::endl;
        printf(entryHelpMessage, argv[0], argv[0]);
    }
}
