// NOT YET WORKING


// #include "../catch.hpp"
// #include "lattice_core.hpp"
// #include "merges/range_mrg.hpp"

// TEST_CASE ( "RangeUnion" ) {
//  using namespace ranges;

//  std::vector<int> lefts({10,20,30});
//  std::vector<int> rights({1,2,3});

//  Lattice ls(views::all(lefts), RangeUnion{});
//  Lattice rs(views::all(rights), RangeUnion{});

//  auto expr = ls + rs + rs;

//  REQUIRE(count(expr.reveal()) == 9);
//  // for (auto &i : lefts) {
//  //  REQUIRE(std::binary_search(expr.reveal().begin(),
// expr.reveal().end(), i));
//  // }
//  // for (auto &i : lefts) {
//  //  REQUIRE(std::binary_search(expr.reveal().begin(),
// expr.reveal().end(), i));
//  // }
// }
