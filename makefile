CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = scheduler
SRCS = Algorithms.cpp
OBJS = $(SRCS:.cpp=.o)

# Folder containing test files
test_inputs = $(wildcard CPU_SCH/*)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@for file in $(test_inputs); do \
		./$(TARGET) < $$file; \
	done

clean:
	rm -f $(OBJS) $(TARGET)
