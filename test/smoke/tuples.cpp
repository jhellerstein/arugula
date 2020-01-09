#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/maxmin_mrg.hpp"
#include "merges/tuple_mrg.hpp"

TEST_CASE("Binary Tuple") {
  Lattice l1(std::make_tuple(Lattice(0,Max{}), Lattice(10,Max{})), TupleMerge{});
  Lattice l2(std::make_tuple(Lattice(1,Max{}), Lattice(5,Max{})), TupleMerge{});

  auto expr = l1 + l2;
  REQUIRE(std::get<0>(expr.reveal()).reveal() 
          == std::get<0>(l2.reveal()).reveal());
  REQUIRE(std::get<1>(expr.reveal()).reveal() 
          == std::get<1>(l1.reveal()).reveal());
}

TEST_CASE("Lexico") {
  Lattice l1(std::make_tuple(Lattice(static_cast<unsigned long long>(0),Max{}), Lattice(10,Max{})), LexicoMax{});
  Lattice l2(std::make_tuple(Lattice(static_cast<unsigned long long>(1),Max{}), Lattice(5,Max{})), LexicoMax{});
  Lattice l3(std::make_tuple(Lattice(static_cast<unsigned long long>(2),Max{}), Lattice(3,Max{})), LexicoMax{});

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