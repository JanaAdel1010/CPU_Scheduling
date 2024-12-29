all: build	run

build: main.cpp
	g++ -std=c++11 -o Lab6 main.cpp algorithm.cpp print.cpp process.cpp

INPUT_DIR = inputs
OUTPUT_DIR = outputs
INPUT_FILES = $(wildcard $(INPUT_DIR)/*.txt)

run: Lab6
	@mkdir -p $(OUTPUT_DIR)  # Ensure the output directory exists
	@for input_file in $(INPUT_FILES); do \
		base_name=$$(basename "$$input_file" .txt); \
		base_name=$$(echo "$$base_name" | sed 's/-input//'); \
		expected_output_file="$(OUTPUT_DIR)/$$base_name-output.txt"; \
		output_file="$(OUTPUT_DIR)/$$base_name-output-test.txt"; \
		./Lab6 < "$$input_file" > "$$output_file"; \
		if diff -q "$$output_file" "$$expected_output_file"; then \
			echo "Test $$base_name passed."; \
		else \
			echo "Test $$base_name failed."; \
		fi; \
	done
