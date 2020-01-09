#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/boolean_mrg.hpp"


TEST_CASE("Binary BoolOr") {

  Lattice f(false, Or{});
  Lattice t(true, Or{});

  auto expr = f + t;
  REQUIRE(expr.reveal() == true);
}

TEST_CASE("Nary BoolOr") {
  Lattice f(false, Or{});
  Lattice t(true, Or{});

  auto expr = f + t;
  REQUIRE(expr.reveal() == true);

  expr = f + t + f + f;
  REQUIRE(expr.reveal() == true);
}


TEST_CASE("Binary BoolAnd") {

  Lattice f(false, And{});
  Lattice t(true, And{});

  auto expr = f + t;
  REQUIRE(expr.reveal() == false);
}

TEST_CASE("Nary BoolAnd") {
  Lattice f(false, And{});
  Lattice t(true, And{});

  auto expr = f + t;
  REQUIRE(expr.reveal() == false);

  expr = f + t + f + f;
  REQUIRE(expr.reveal() == false);
}
