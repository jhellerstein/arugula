#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/vector_mrg.hpp"

TEST_CASE("VectorUnion") {
  std::vector<int> lefts({1, 20, 30});
  std::vector<int> rights({1, 2, 3});

  Lattice ls(lefts, VectorUnion{});
  Lattice rs(rights, VectorUnion{});

  auto expr = ls + rs + rs;

  REQUIRE(expr.reveal().size() == 5);
  for (auto &i : lefts) {
    REQUIRE(std::binary_search(expr.reveal().begin(), expr.reveal().end(), i));
  }
  for (auto &i : lefts) {
    REQUIRE(std::binary_search(expr.reveal().begin(), expr.reveal().end(), i));
  }
}

TEST_CASE("VectorUnionUnique") {
  // This fails to compile with std::vector<std::unique_ptr<int> >
  std::vector<std::shared_ptr<int>> lefts;
  std::vector<std::shared_ptr<int>> rights;

  for (auto i : {1,20,30})
    lefts.push_back(std::move(std::make_unique<int>(i)));
  for (auto i : {1,2,3})
    rights.push_back(std::move(std::make_unique<int>(i)));
  Lattice ls(lefts, VectorUnion{});
  Lattice rs(rights, VectorUnion{});

  auto expr = ls + rs;

  REQUIRE(expr.reveal().size() == 5);
  for (auto i : {1,20,30}) {
    REQUIRE(std::binary_search(expr.reveal().begin(), 
                   expr.reveal().end(), 
                             std::make_shared<int>(i), 
                             pcmp<const std::shared_ptr<int>>));
  }
  for (auto i : {1,2,3}) {
    REQUIRE(std::binary_search(expr.reveal().begin(), 
                   expr.reveal().end(), 
                             std::make_shared<int>(i), 
                             pcmp<const std::shared_ptr<int>>));
  }
}