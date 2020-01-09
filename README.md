# arugula
This is a small C++ lattice library using expression templates. The basic template of a lattice is `Lattice<T, Func>` where `T` is some C++ type, and `Func` is a functor to do the lattice join (i.e. merge) operation, which is represented as `operator+()` (or `operator+=`).

Given this structure, most of the logic is in the merge functors in `include/merges`.

### A simple example
For example, the code below builds two lattices `lefts` and `rights`. Each has the C++ type is `Lattice<std::set<int>, Union`, or in mathematical terms a join semi-lattice *(P(Z), U)*.

```c++
  std::set<int> lefts({1, 20, 30});
  std::set<int> rights({1, 2, 3});

  Lattice ls(lefts, Union{});
  Lattice rs(rights, Union{});

  Lattice<std::set<int>, Union> expr = ls + rs;
```

### Lattice composition
The `test/smoke` directory shows many more examples of how to build lattices using standard C++ types and the merge functors. Among the more interesting examples are lattices composed of other lattices, e.g. in `include/merges/vector_clock_mrg.h`:

```c++
using VectorClock = Lattice<std::map<std::string, Lattice<unsigned, Max> >, MapUnion>;
```

A `VectorClock` is a Lattice based on a map data structure, where the key is a `std::string`, and the value is a nested lattice (of `unsigned` under `Max`). The merge function for map lattices recursively merges the values using the type of the nested lattice.

You can of course compose the `VectorClock` lattice into a superlattice, e.g. for causality tracking as shown in the `test` directory.