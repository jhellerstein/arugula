INCLUDE=include
CXX=g++
CFLAGS=-I${INCLUDE} --std=c++17 -g -DCATCH_CONFIG_ENABLE_BENCHMARKING # -fsanitize=address -fno-omit-frame-pointer
LDFLAGS = 
DEPS = $(wildcard $(INCLUDE)/*.hpp) $(wildcard $(INCLUDE)/*/*.hpp)
SMOKE_SRC = $(wildcard test/smoke/*.cpp)
SMOKE_OBJ = $(SMOKE_SRC:.cpp=.o)
PERF_SRC = $(wildcard test/perf/*.cpp)
PERF_OBJ = $(PERF_SRC:.cpp=.o)
OBJ = ${SMOKE_OBJ} ${PERF_OBJ}
BINARIES = smoke perf

print-%: ; @echo $* is $($*)

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

all: smoke perf

smoke: $(SMOKE_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

perf: $(PERF_OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

valgrind:  $(SMOKE_OBJ)
	valgrind --leak-check=yes ./smoke

.PHONY: clean

clean:
	rm -f $(OBJ) $(BINARIES)
