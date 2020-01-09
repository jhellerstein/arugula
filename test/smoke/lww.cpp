#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/lww_mrg.hpp"

// Anna-like Last-Writer-Wins lattice
TEST_CASE("LWW") {
  Lattice l1(std::make_tuple(Lattice(static_cast<unsigned long long>(0),Max{}), Lattice(10,Max{})), LWWMerge{});
  Lattice l2(std::make_tuple(Lattice(static_cast<unsigned long long>(1),Max{}), Lattice(5,Max{})), LWWMerge{});
  Lattice l3(std::make_tuple(Lattice(static_cast<unsigned long long>(2),Max{}), Lattice(3,Max{})), LWWMerge{});

  auto expr = l1 + l2;
  REQUIRE(std::get<0>(expr.reveal()).reveal() 
          == std::get<0>(l2.reveal()).reveal());
  REQUIRE(std::get<1>(expr.reveal()).reveal() 
          == std::get<1>(l2.reveal()).reveal());
  expr += l3;
  REQUIRE(std::get<0>(expr.reveal()).reveal() 
          == std::get<0>(l3.reveal()).reveal());
  REQUIRE(std::get<1>(expr.reveal()).reveal() 
          == std::get<1>(l3.reveal()).reveal());
}