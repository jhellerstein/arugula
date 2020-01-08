#ifndef VECTOR_CLOCK_MRG_H
#define VECTOR_CLOCK_MRG_H

#include "maxmin_mrg.h"
#include "map_mrg.h"
using VectorClock = Lattice<std::map<std::string, Lattice<unsigned, Max> >, MapUnion>;

#endif // VECTOR_CLOCK_MRG_H