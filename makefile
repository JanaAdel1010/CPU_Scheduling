all:build

build: Algorithms.cpp

	g++	-std=c++11	-o	Algorithm	main.cpp	algorithm.cpp	print.cpp	process.cpp
INPUT_DIR = inputs
OUTPUT_DIR = outputs
INPUT_FILES = $(wildcard $(INPUT_DIR)/*.txt) 	


run: Algorithm
	@for input_file in $(INPUT_FILES); do \
		base_name=$$(basename $$input_file .txt); \
		base_name=$$(echo $$base_name | sed 's/-input//'); \
		expected_output_file=$(OUTPUT_DIR)/$$base_name-output.txt; \
		output_file=$(OUTPUT_DIR)/$$base_name-output.txt; \
		./Algorithm < $$input_file > $$output_file; \
		if diff -q $$output_file $$expected_output_file; then \
			echo "Test $$base_name passed."; \
		else \
			echo "Test $$base_name failed."; \
		fi \
	done