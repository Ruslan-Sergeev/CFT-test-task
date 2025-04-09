CC = gcc
CFLAGS = -Wall -Wextra -O3

PROCESSING_UTILITY_OBJ = processing_utility.o avl_tree.o
TEST_OBJ = test_utility.o

TARGETS = processing_utility test_utility

all: $(TARGETS)

processing_utility: $(PROCESSING_UTILITY_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

test_utility: $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROCESSING_UTILITY_OBJ) $(TEST_OBJ) $(TARGETS)