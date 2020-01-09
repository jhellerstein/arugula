#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/map_mrg.hpp"
#include "merges/maxmin_mrg.hpp"
#include "merges/setop_mrg.hpp"

TEST_CASE("Map<IntMax>") {
  std::map<std::string, Lattice<int, Max>> leftm = {{"x", Lattice(2, Max{})},
                                                    {"y", Lattice(4, Max{})}};

  std::map<std::string, Lattice<int, Max>> rightm = {{"x", Lattice(3, Max{})},
                                                     {"z", Lattice(5, Max{})}};

  Lattice lm(leftm, MapUnion{});
  Lattice rm(rightm, MapUnion{});
  auto expr = lm + rm;
  auto exprmap = expr.reveal();
  REQUIRE(exprmap.size() == 3);
  REQUIRE(exprmap.at("x").reveal() == 3);
  REQUIRE(exprmap.at("y").reveal() == 4);
  REQUIRE(exprmap.at("z").reveal() == 5);
}

TEST_CASE("Map<SetUnion>") {
  auto lefts = std::set<int>{10, 20, 30};
  auto rights = std::set<int>{1, 2, 3};

  std::map<std::string, Lattice<std::set<int>, Union>> leftm2 = {
      {"x", Lattice(lefts, Union{})}, {"y", Lattice(rights, Union{})}};

  std::map<std::string, Lattice<std::set<int>, Union>> rightm2 = {
      {"x", Lattice(rights, Union{})}, {"z", Lattice(lefts, Union{})}};

  Lattice lm2(leftm2, MapUnion{});
  Lattice rm2(rightm2, MapUnion{});
  auto expr = lm2 + rm2;
  auto exprmap = expr.reveal();
  REQUIRE(exprmap.size() == 3);
  REQUIRE(exprmap.at("x").reveal().size() == 6);
  for (auto &i : lefts) {
    REQUIRE(exprmap.at("x").reveal().find(i) != exprmap.at("x").reveal().end());
  }
  for (auto &i : lefts) {
    REQUIRE(exprmap.at("x").reveal().find(i) != exprmap.at("x").reveal().end());
  }
  REQUIRE(exprmap.at("y").reveal() == rights);
  REQUIRE(exprmap.at("z").reveal() == lefts);
}
