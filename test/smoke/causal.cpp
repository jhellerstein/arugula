#include "../catch.hpp"

#include "lattice_core.hpp"
#include "merges/causal_mrg.hpp"
#include "merges/setop_mrg.hpp"
#include "merges/vector_clock_mrg.hpp"


TEST_CASE("Single key causal") {
  //  using VectorClock = Lattice<std::map<std::string, Lattice<unsigned, Max> >, MapUnion>;
  VectorClock vc1({{"x", Lattice(static_cast<unsigned>(2), Max{})},
                   {"y", Lattice(static_cast<unsigned>(4), Max{})}});
  VectorClock vc2({{"x", Lattice(static_cast<unsigned>(3), Max{})},
                   {"y", Lattice(static_cast<unsigned>(5), Max{})}});
  VectorClock vc3({{"x", Lattice(static_cast<unsigned>(1), Max{})},
                   {"y", Lattice(static_cast<unsigned>(6), Max{})}});
  const std::set<std::string> jset({"Joe"});
  const std::set<std::string> bset({"Bob"});
  const std::set<std::string> aset({"Alice"});
  Lattice l1(std::make_tuple(vc1, Lattice(jset, Union{})), CausalMerge{});
  Lattice l2(std::make_tuple(vc2, Lattice(bset, Union{})), CausalMerge{});
  Lattice l3(std::make_tuple(vc3, Lattice(aset, Union{})), CausalMerge{});

  CausalMerge m;
  auto expr = l1+l2;
  REQUIRE(expr == l2);
  expr = l2+l3;
  REQUIRE(expr != l2);
  REQUIRE(expr != l3);
}