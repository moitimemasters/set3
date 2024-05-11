import typing as tp
import os
import argparse

import matplotlib.pyplot as plt

TestCase: tp.TypeAlias = str
ArraySize: tp.TypeAlias = int
Measurement: tp.TypeAlias = tuple[int, int]
ArrayMeasurements: tp.TypeAlias = list[Measurement]
CollectedMeasurements: tp.TypeAlias = dict[TestCase, dict[ArraySize, ArrayMeasurements]]


def parse_measurements(file_path: str) -> CollectedMeasurements:
    measurements: CollectedMeasurements = {}
    current_test_case = None
    current_size = None

    with open(file_path, "r") as file:
        for line in file:
            if line.startswith("/ "):
                if ":" in line:
                    current_size = int(line.strip("/ :\n"))
                    if not current_test_case:
                        continue
                    if current_test_case not in measurements:
                        measurements[current_test_case] = {}
                    measurements[current_test_case][current_size] = []
                else:
                    current_test_case = line.strip("/ \n")
            elif line == "\n":
                if current_test_case == "almost sorted" and current_size == 3000:
                    break
                continue
            else:
                time, comparisons = map(int, line.split())
                if not current_test_case or not current_size:
                    continue
                measurements[current_test_case][current_size].append((time, comparisons))

    return measurements


def plot_graphs(measurements: CollectedMeasurements, output_file: str) -> None:
    for test_case, data in measurements.items():
        sizes = sorted(data.keys())
        avg_times = [sum(t for t, _ in data[size]) / len(data[size]) for size in sizes]
        avg_comparisons = [sum(c for _, c in data[size]) / len(data[size]) for size in sizes]

        plt.figure()
        plt.plot(sizes, avg_times)
        plt.xlabel("Array Size")
        plt.ylabel("Average Time (microseconds)")
        plt.title(f"{test_case} - Average Time")
        plt.tight_layout()
        plt.savefig(f"images/{output_file}_{test_case}_time.png")
        plt.close()

        plt.figure()
        plt.plot(sizes, avg_comparisons)
        plt.xlabel("Array Size")
        plt.ylabel("Average Comparisons")
        plt.title(f"{test_case} - Average Comparisons")
        plt.tight_layout()
        plt.savefig(f"images/{output_file}_{test_case}_comparisons.png")
        plt.close()


def main():
    parser = argparse.ArgumentParser(description="Parse sorting measurements and generate graphs.")
    parser.add_argument("file_path", help="Path to the measurements file")
    args = parser.parse_args()

    input_file = args.file_path
    output_file = os.path.splitext(os.path.basename(input_file))[0]

    if not os.path.exists("images"):
        os.makedirs("images")

    measurements = parse_measurements(input_file)
    plot_graphs(measurements, output_file)


if __name__ == "__main__":
    main()
