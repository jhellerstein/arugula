#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"

// unsigned int Factorial( unsigned int number ) {
//     return number <= 1 ? number : Factorial(number-1)*number;
// }

// TEST_CASE( "Factorials are computed", "[factorial]" ) {
// 	REQUIRE( Factorial(0) == 1 );
//     REQUIRE( Factorial(1) == 1 );
//     REQUIRE( Factorial(2) == 2 );
//     REQUIRE( Factorial(3) == 6 );
//     REQUIRE( Factorial(10) == 3628800 );
// }

#include "collection_io.h"
#include "lattice_core.h"
#include "lattice_merge.h"
#include <iostream>
#include <map>
#include <set>

TEST_CASE("Binary IntMax") {

  const Max m;
  Lattice li(1, Max{});
  Lattice ri(40, Max{});

  auto expr = li + ri;
  REQUIRE(expr.reveal() == 40);
}

TEST_CASE("Nary IntMax") {
  Lattice li(1, Max{});
  Lattice ri(40, Max{});
  Lattice ti(50, Max{});

  auto expr = li + ri + ti + li + ri;
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

TEST_CASE("SetUnion") {
  std::set<int> lefts({1, 20, 30});
  std::set<int> rights({1, 2, 3});

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

// TEST_CASE ( "RangeUnion" ) {
// 	using namespace ranges;

// 	std::vector<int> lefts({10,20,30});
// 	std::vector<int> rights({1,2,3});

// 	Lattice ls(views::all(lefts), RangeUnion{});
// 	Lattice rs(views::all(rights), RangeUnion{});

// 	auto expr = ls + rs + rs;

// 	REQUIRE(count(expr.reveal()) == 9);
// 	// for (auto &i : lefts) {
// 	// 	REQUIRE(std::binary_search(expr.reveal().begin(),
// expr.reveal().end(), i));
// 	// }
// 	// for (auto &i : lefts) {
// 	// 	REQUIRE(std::binary_search(expr.reveal().begin(),
// expr.reveal().end(), i));
// 	// }
// }


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