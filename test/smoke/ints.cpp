#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/maxmin_mrg.hpp"

TEST_CASE("Binary IntMax") {

  const Max m;
  Lattice li(1, Max{});
  Lattice ri(40, Max{});

  auto expr = li + ri;
  REQUIRE(expr.reveal() == 40);
}

TEST_CASE("Nary IntMax") {
  Lattice li(1, Max{});
  Lattice lj(40, Max{});
  Lattice lk(50, Max{});

  auto expr = li + lj + lk + li + lj;
  REQUIRE(expr.reveal() == 50);
}

TEST_CASE("IntMax merges") {
  Lattice li(1, Max{});
  Lattice li3(3, Max{});
  li += 2;
  REQUIRE(li.reveal() == 2);
  li += li3;
  REQUIRE(li.reveal() == 3);
}

TEST_CASE("IntMax equality") {
  Lattice l1(1,Max{});
  Lattice l2(2,Max{});
  Lattice l11(1,Max{});
  REQUIRE(l1 == l11);
  REQUIRE(!(l1 == l2));
  REQUIRE(l1 != l2);
}

TEST_CASE("Binary IntMin") {

  const Min m;
  Lattice li(1, Min{});
  Lattice lj(40, Min{});

  auto expr = li + lj;
  REQUIRE(expr.reveal() == 1);
}

TEST_CASE("Nary IntMin") {
  Lattice li(1, Min{});
  Lattice lj(40, Min{});
  Lattice lk(50, Min{});

  auto expr = li + lj + lk + li + lj;
  REQUIRE(expr.reveal() == 1);
}

TEST_CASE("IntMin merges") {
  Lattice li(1, Min{});
  Lattice li3(3, Min{});
  li3 += 2;
  REQUIRE(li3.reveal() == 2);
  li3 += li;
  REQUIRE(li.reveal() == 1);
}

TEST_CASE("IntMin equality") {
  Lattice l1(1,Min{});
  Lattice l2(2,Min{});
  Lattice l11(1,Min{});
  REQUIRE(l1 == l11);
  REQUIRE(!(l1 == l2));
  REQUIRE(l1 != l2);
}
