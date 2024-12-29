all: build	run

build: main.cpp
	g++ -std=c++11 -o lab6 main.cpp algorithm.cpp print.cpp process.cpp

INPUT_FILES = $(wildcard testcases/*-input.txt)
OUTPUT_FILES = $(patsubst testcases/%-input.txt,testcases/%-output.txt,$(INPUT_FILES))

run: build
	@for input_file in $(INPUT_FILES); do \
		base_name=$$(basename "$$input_file" -input.txt); \
		expected_output_file="testcases/$$base_name-output.txt"; \
		output_file="testcases/$$base_name-output-test.txt"; \
		./lab6 < "$$input_file" > "$$output_file"; \
		if diff -q "$$output_file" "$$expected_output_file" > /dev/null; then \
			echo "Test $$base_name passed."; \
		else \
			echo "Test $$base_name failed."; \
			echo "Differences:"; \
			diff "$$output_file" "$$expected_output_file"; \
		fi; \
	done

clean:
	rm -f lab6 testcases/*-output-test.txt
