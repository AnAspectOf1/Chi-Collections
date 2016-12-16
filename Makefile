CC = g++
CC_FLAGS = -std=c++11 -Wall
CHI_INCL_DIR = include

TEST_SRC_FILES = $(shell find test -name *.cpp)
TEST_BIN_FILES = $(TEST_SRC_FILES:.cpp=)

test: $(TEST_BIN_FILES)
	$(TEST_BIN_FILES)

clean:
	rm -f $(TEST_BIN_FILES)

test/%: test/%.cpp
	$(CC) -o $@ $(CC_FLAGS) -I $(CHI_INCL_DIR)  $<
