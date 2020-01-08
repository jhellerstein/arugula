CC=g++
CFLAGS=--std=c++17 -g # -fsanitize=address -fno-omit-frame-pointer
OBJ = test.o
MRG_DIR = "merges"
MERGES = $(shell find $(MRG_DIR) -name "*.h" -type f)
UTILS_DIR = "merges"
UTILS = $(shell find $(UTILS_DIR) -name "*.h" -type f)

DEPS = lattice_core.h lattice_merge.h binexpr.h
BINARIES = test

%.o: %.cc $(DEPS) ${MERGES} ${UTILS}
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

valgrind:  $(test)
	valgrind --leak-check=yes ./test

.PHONY: clean

clean:
	rm -f *.o $(BINARIES)