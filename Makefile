INCLUDE = "include"
CXX=g++
CFLAGS=-I${INCLUDE} --std=c++17 -g # -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = 
DEPS = $(wildcard $(INCLUDE)/*.hpp) $(wildcard $(INCLUDE)/*/*.hpp)
CPPSRC = $(wildcard test/*.cpp)
OBJ = $(CPPSRC:.cpp=.o)
BINARIES = lattice_test

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

lattice_test: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

valgrind:  $(BINARIES)
	valgrind --leak-check=yes ./$(BINARIES)

.PHONY: clean

clean:
	rm -f $(OBJ) $(BINARIES)