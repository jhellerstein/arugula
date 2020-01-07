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

TEST_CASE("Binary Tuple") {
  Lattice l1(std::make_tuple(Lattice(0,Max{}), Lattice(10,Max{})), TupleMerge{});
  Lattice l2(std::make_tuple(Lattice(1,Max{}), Lattice(5,Max{})), TupleMerge{});

  auto expr = l1 + l2;
  REQUIRE(std::get<0>(expr.reveal()).reveal() 
          == std::get<0>(l2.reveal()).reveal());
  REQUIRE(std::get<1>(expr.reveal()).reveal() 
          == std::get<1>(l1.reveal()).reveal());
}

// From Anna
TEST_CASE("LWW") {
  Lattice l1(std::make_tuple(Lattice(0,Max{}), Lattice(10,Max{})), LWWMerge{});
  Lattice l2(std::make_tuple(Lattice(1,Max{}), Lattice(5,Max{})), LWWMerge{});
  Lattice l3(std::make_tuple(Lattice(2,Max{}), Lattice(3,Max{})), LWWMerge{});

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