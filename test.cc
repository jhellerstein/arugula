#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
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

#include <iostream>
#include <map>
#include <set>
#include "lattice_core.h"
#include "lattice_merge.h"
#include "collection_io.h"


TEST_CASE( "Binary IntMax" ) {

	Lattice li(1, Max{});
	Lattice ri(40, Max{});

	auto expr = li + ri;
	REQUIRE(expr.reveal() == 40);
}

TEST_CASE ( "Nary IntMax" ) {
	Lattice li(1, Max{});
	Lattice ri(40, Max{});
	Lattice ti(50, Max{});

	auto expr = li + ri + ti + li + ri;
	REQUIRE(expr.reveal() == 50);
}

TEST_CASE ( "SetUnion" ) {
	// This doesn't compile!
	// std::set<std::shared_ptr<int> > lefts;
	// std::set<std::shared_ptr<int> > rights;

	// lefts.insert(std::make_unique<int>(10));
	// lefts.insert(std::make_unique<int>(20));
	// lefts.insert(std::make_unique<int>(30));
	// rights.insert(std::make_unique<int>(1));
	// rights.insert(std::make_unique<int>(2));
	// rights.insert(std::make_unique<int>(3));

	std::set<int> lefts({10,20,30});
	std::set<int> rights({1,2,3});

	Lattice ls(lefts, Union{});
	Lattice rs(rights, Union{});

	auto expr = ls + rs;

	REQUIRE(expr.reveal().size() == 6);
	for (auto &i : lefts) {
		REQUIRE(expr.reveal().find(i) != expr.reveal().end());
	}
	for (auto &i : lefts) {
		REQUIRE(expr.reveal().find(i) != expr.reveal().end());
	}
}

TEST_CASE ( "VectorUnion" ) {
	std::vector<int> lefts({10,20,30});
	std::vector<int> rights({1,2,3});

	Lattice ls(lefts, VectorUnion{});
	Lattice rs(rights, VectorUnion{});

	auto expr = ls + rs;

	REQUIRE(expr.reveal().size() == 6);
	for (auto &i : lefts) {
		REQUIRE(std::binary_search(expr.reveal().begin(), expr.reveal().end(), i));
	}
	for (auto &i : lefts) {
		REQUIRE(std::binary_search(expr.reveal().begin(), expr.reveal().end(), i));
	}
}

TEST_CASE ( "VectorUnionUnique" ) {
	// This fails to compile with std::vector<std::unique_ptr<int> >
	std::vector<std::shared_ptr<int> > lefts;
	std::vector<std::shared_ptr<int> > rights;

	lefts.push_back(std::move(std::make_unique<int>(10)));
	lefts.push_back(std::move(std::make_unique<int>(20)));
	lefts.push_back(std::move(std::make_unique<int>(30)));
	rights.push_back(std::move(std::make_unique<int>(1)));
	rights.push_back(std::move(std::make_unique<int>(2)));
	rights.push_back(std::move(std::make_unique<int>(3)));
	Lattice ls(lefts, VectorUnion{});
	Lattice rs(rights, VectorUnion{});

	auto expr = ls + rs;

	REQUIRE(expr.reveal().size() == 6);
	for (auto &i : lefts) {
		REQUIRE(std::binary_search(expr.reveal().begin(), expr.reveal().end(), i));
	}
	for (auto &i : lefts) {
		REQUIRE(std::binary_search(expr.reveal().begin(), expr.reveal().end(), i));
	}
}

TEST_CASE ( "Map<IntMax>") {
	std::map<std::string, Lattice<int, Max> > leftm = {
		{"x", Lattice(2, Max{})},
		{"y", Lattice(4, Max{})}
	};

	std::map<std::string, Lattice<int, Max> > rightm = {
		{"x", Lattice(3, Max{})},
		{"z", Lattice(5, Max{})}
	};

	Lattice lm(leftm, MapUnion{});
	Lattice rm(rightm, MapUnion{});
	auto expr = lm + rm;
	auto exprmap = expr.reveal();
	REQUIRE(exprmap.size() == 3);
	REQUIRE(exprmap.at("x").reveal() == 3);
	REQUIRE(exprmap.at("y").reveal() == 4);
	REQUIRE(exprmap.at("z").reveal() == 5);
}

TEST_CASE ( "Map<SetUnion>") {
	auto lefts = std::set<int> {10, 20, 30};
	auto rights = std::set<int> {1, 2, 3};

	std::map<std::string, Lattice<std::set<int>, Union> > leftm2 = {
		{"x", Lattice(lefts, Union{})},
		{"y", Lattice(rights, Union{})}
	};

	std::map<std::string, Lattice<std::set<int>, Union> > rightm2 = {
		{"x", Lattice(rights, Union{})},
		{"z", Lattice(lefts, Union{})}
	};

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