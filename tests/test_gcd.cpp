#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <example/example.hpp>

using example::gcd;

TEST_CASE("gcd of two equal numbers is the number", "[gcd]" ) {
  REQUIRE(gcd(42, 42) == 42);
}
