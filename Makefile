CC=g++
CFLAGS=--std=c++17 -g # -fsanitize=address -fno-omit-frame-pointer
OBJ = test.o
DEPS = lattice_core.h lattice_merge.h binexpr.h collection_io.h
BINARIES = test

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

valgrind:  $(test)
	valgrind --leak-check=yes ./test

.PHONY: clean

clean:
	rm -f *.o $(BINARIES)