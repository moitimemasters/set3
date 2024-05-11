CXX = g++
CXXFLAGS = -std=c++23 -O0
SORT_SEED ?= 1337
SORT_TRIES ?= 30
TEST_CASE_NAME ?= test_cases_$(SORT_SEED).in
SORT_ALGORITHMS = qs ms sqs sms msd mqs

.PHONY: all clean gen test

all: bin/main

bin/main: main.cpp StringGenerator/StringGenerator.cpp
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@

gen: bin/main
	@mkdir -p test_cases
	./bin/main gen -s $(SORT_SEED) -o test_cases/$(TEST_CASE_NAME)

test: bin/main
	@mkdir -p sort_results
	@for alg in $(SORT_ALGORITHMS); do \
		./bin/main sort -a $${alg} -n $(SORT_TRIES) -i test_cases/$(TEST_CASE_NAME) -o sort_results/$${alg}_$(SORT_TRIES).out; \
		python plot.py sort_results/$${alg}_$(SORT_TRIES).out; \
	done

clean:
	rm -rf bin test_cases sort_results images
