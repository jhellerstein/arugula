#include "../catch.hpp"
#include "lattice_core.hpp"
#include "merges/setop_mrg.hpp"

TEST_CASE("SetUnion") {
  std::set<int> lefts({1, 20, 30});
  std::set<int> rights({1, 2, 3});

  Lattice ls(lefts, Union{});
  Lattice rs(rights, Union{});

  Lattice<std::set<int>, Union> expr = ls + rs;

  REQUIRE(expr.reveal().size() == 5);
  for (auto &i : lefts) {
    REQUIRE(expr.reveal().find(i) != expr.reveal().end());
  }
  for (auto &i : lefts) {
    REQUIRE(expr.reveal().find(i) != expr.reveal().end());
  }
}

TEST_CASE("SetUnion merges") {
  std::set<int> lefts({1, 20, 30});
  std::set<int> rights({1, 2, 3});
  std::set<int> others({1, 200, 300});

  Lattice ls(std::move(lefts), Union{});
  Lattice rs(std::move(rights), Union{});

  ls += rs;
  REQUIRE(ls.reveal().size() == 5);
  for (auto i : {1,2,3,20,30}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }

  ls += others;
  REQUIRE(ls.reveal().size() == 7);
  for (auto i : {1,2,3,20,30,200,300}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }
}

TEST_CASE("SetUnion Equality") {
  std::set<int> lefts({1, 20, 30});
  std::set<int> rights({20, 1, 30});
  std::set<int> evens({2, 4, 6});

  Lattice ls(lefts, Union{});
  Lattice rs(rights, Union{});
  Lattice es(evens, Union{});

  REQUIRE(ls == rs);
  REQUIRE(ls != es);
}

TEST_CASE("SetUnion Assign") {
  std::set<int> negnums({-1,-2,-3});
  std::set<int> nums({1,2,3});
  std::set<int> mt;

  Lattice ls(std::move(mt), Union{});
  ls = negnums;
  REQUIRE(ls.reveal().size() == 3);
  for (auto i : negnums) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }  
  REQUIRE(negnums.size() == 3);
  Lattice ls2(std::move(nums), Union{});
  ls = ls2;
  REQUIRE(ls.reveal().size() == 3);
  for (auto i : {1,2,3}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }  
}

TEST_CASE("SetUnion Wrap") {
  std::set<int> negnums({-1,-2,-3});
  std::set<int> nums({1,2,3});
  std::set<int> mt;

  Lattice ls(std::move(mt), Union{});
  ls.wrap(std::move(negnums));
  REQUIRE(ls.reveal().size() == 3);
  REQUIRE(negnums.size() == 0);
  ls.wrap(std::move(nums));
  REQUIRE(ls.reveal().size() == 3);
  REQUIRE(nums.size() == 0);
  for (auto i : {1,2,3}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }  
}

TEST_CASE("SetIntersect") {
  std::set<int> lefts({1, 2, 30});
  std::set<int> rights({1, 2, 3});

  Lattice ls(lefts, Intersect{});
  Lattice rs(rights, Intersect{});

  auto expr = ls + rs;

  REQUIRE(expr.reveal().size() == 2);
  for (auto &i : {1, 2}) {
    REQUIRE(expr.reveal().find(i) != expr.reveal().end());
  }
}

TEST_CASE("SetIntersect merges") {
  std::set<int> lefts({1, 2, 30});
  std::set<int> rights({1, 2, 3});
  std::set<int> others({2, 300});

  Lattice ls(std::move(lefts), Intersect{});
  Lattice rs(std::move(rights), Intersect{});

  ls += rs;
  REQUIRE(ls.reveal().size() == 2);
  for (auto i : {1,2}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }

  ls += others;
  REQUIRE(ls.reveal().size() == 1);
  REQUIRE(ls.reveal().find(2) != ls.reveal().end());
}

TEST_CASE("UnorderedSetUnion") {
  std::unordered_set<int> lefts({1, 20, 30});
  std::unordered_set<int> rights({1, 2, 3});

  Lattice ls(lefts, Union{});
  Lattice rs(rights, Union{});

  auto expr = ls + rs;

  REQUIRE(expr.reveal().size() == 5);
  for (auto &i : lefts) {
    REQUIRE(expr.reveal().find(i) != expr.reveal().end());
  }
  for (auto &i : lefts) {
    REQUIRE(expr.reveal().find(i) != expr.reveal().end());
  }
}

TEST_CASE("UnorderedSetUnion merges") {
  std::unordered_set<int> lefts({1, 20, 30});
  std::unordered_set<int> rights({1, 2, 3});
  std::unordered_set<int> others({1, 200, 300});

  Lattice ls(std::move(lefts), Union{});
  Lattice rs(std::move(rights), Union{});

  ls += rs;
  REQUIRE(ls.reveal().size() == 5);
  for (auto i : {1,2,3,20,30}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }

  ls += others;
  REQUIRE(ls.reveal().size() == 7);
  for (auto i : {1,2,3,20,30,200,300}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }
}

TEST_CASE("UnorderedSetUnion Equality") {
  std::unordered_set<int> first_order({1, 20, 30});
  std::unordered_set<int> first_unorder({20, 1, 30});
  std::unordered_set<int> evens({2, 4, 6});
  std::unordered_set<int> second_order({1, 20, 30});

  Lattice lfo(first_order, Union{});
  Lattice lfu(first_unorder, Union{});
  Lattice le(evens, Union{});
  Lattice lso(second_order, Union{});
  
  REQUIRE(lfo != lfu);
  REQUIRE(lfo != le);
  REQUIRE(lfo == lso);
}

TEST_CASE("UnorderedSetUnion Assign") {
  std::unordered_set<int> negnums({-1,-2,-3});
  std::unordered_set<int> nums({1,2,3});
  std::unordered_set<int> mt;

  Lattice ls(std::move(mt), Union{});
  ls = negnums;
  REQUIRE(ls.reveal().size() == 3);
  for (auto i : negnums) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }  
  REQUIRE(negnums.size() == 3);
  Lattice ls2(std::move(nums), Union{});
  ls = ls2;
  REQUIRE(ls.reveal().size() == 3);
  for (auto i : {1,2,3}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }  
}

TEST_CASE("UnorderedSetUnion Wrap") {
  std::unordered_set<int> negnums({-1,-2,-3});
  std::unordered_set<int> nums({1,2,3});
  std::unordered_set<int> mt;

  Lattice ls(std::move(mt), Union{});
  ls.wrap(std::move(negnums));
  REQUIRE(ls.reveal().size() == 3);
  REQUIRE(negnums.size() == 0);
  ls.wrap(std::move(nums));
  REQUIRE(ls.reveal().size() == 3);
  REQUIRE(nums.size() == 0);
  for (auto i : {1,2,3}) {
    REQUIRE(ls.reveal().find(i) != ls.reveal().end());
  }  
}

TEST_CASE("SetUnionUnique") {
  // This fails to compile with std::set<std::unique_ptr<int> >
  std::set<std::shared_ptr<int>, PointerCmp> lefts;
  std::set<std::shared_ptr<int>, PointerCmp> rights;

  for (auto n : {1,20,30})
    lefts.insert(std::make_unique<int>(n));
  for (auto n : {1, 2, 3})
    rights.insert(std::make_unique<int>(n));

  Lattice ls(std::move(lefts), Union{});
  Lattice rs(std::move(rights), Union{});

  auto expr = ls + rs;

  REQUIRE(expr.reveal().size() == 5);
  for (auto i : {1,2,3,20,30}) {
    REQUIRE(expr.reveal().find(std::make_unique<int>(i)) != expr.reveal().end());
  }
}

TEST_CASE("SetUnionUnique merges") {
  std::set<std::shared_ptr<int>, PointerCmp> lefts;
  std::set<std::shared_ptr<int>, PointerCmp> rights;
  std::set<std::shared_ptr<int>, PointerCmp> others;

  for (auto n : {1,20,30})
    lefts.insert(std::make_unique<int>(n));
  for (auto n : {1, 2, 3})
    rights.insert(std::make_unique<int>(n));

  Lattice ls(std::move(lefts), Union{});
  Lattice rs(std::move(rights), Union{});

  ls += rs;
  REQUIRE(ls.reveal().size() == 5);
  for (auto i : {1,2,3,20,30}) {
    REQUIRE(ls.reveal().find(std::make_unique<int>(i)) != ls.reveal().end());
  }

  for (auto n : {1, 200, 300})
    others.insert(std::make_unique<int>(n));

  ls += others;
  REQUIRE(ls.reveal().size() == 7);
  for (auto i : {1,2,3,20,30,200,300}) {
    REQUIRE(ls.reveal().find(std::make_unique<int>(i)) != ls.reveal().end());
  }
}

TEST_CASE("SetUnionUnique Equality") {
  std::set<std::shared_ptr<int>, PointerCmp> lefts;
  std::set<std::shared_ptr<int>, PointerCmp> rights;
  std::set<std::shared_ptr<int>, PointerCmp> others;

  for (auto n : {1,20,30})
    lefts.insert(std::make_unique<int>(n));
  for (auto n : {20, 1, 30})
    rights.insert(std::make_unique<int>(n));
  for (auto n : {1, 2, 3})
    others.insert(std::make_unique<int>(n));
  // while these should be equal, C++ uses pointer equality and fails! Bummer!
  REQUIRE(lefts != rights);

  // Nonetheless, Lattice equality does the right comparison for ordered stl containers!  
  Lattice ls(std::move(lefts), Union{});
  Lattice rs(std::move(rights), Union{});
  Lattice oss(std::move(others), Union{});
  REQUIRE(ls == rs);
  REQUIRE(ls != oss);
}
