INCLUDE = "include"
CXX=g++
CFLAGS=-I${INCLUDE} --std=c++17 -g # -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = 
DEPS = $(wildcard $(INCLUDE)/*.hpp) $(wildcard $(INCLUDE)/*/*.hpp)
CCSRC = $(wildcard test/*.cc)
OBJ = $(CCSRC:.cc=.o)
BINARIES = lattice_test

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

lattice_test: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

valgrind:  $(test)
	valgrind --leak-check=yes lattice_test

.PHONY: clean

clean:
	rm -f $(OBJ) $(BINARIES)