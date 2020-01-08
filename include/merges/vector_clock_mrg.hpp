#ifndef VECTOR_CLOCK_MRG_H
#define VECTOR_CLOCK_MRG_H

#include "maxmin_mrg.hpp"
#include "map_mrg.hpp"
using VectorClock = Lattice<std::map<std::string, Lattice<unsigned, Max> >, MapUnion>;

#endif // VECTOR_CLOCK_MRG_H