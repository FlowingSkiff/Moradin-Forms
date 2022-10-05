#include <catch2/catch.hpp>

#include "lib.hpp"

TEST_CASE("Name is Moradin-Forms", "[library]")
{
  library lib;
  REQUIRE(lib.name == "Moradin-Forms");
}
