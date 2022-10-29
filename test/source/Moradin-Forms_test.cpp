#include <catch2/catch_all.hpp>

#include "Core/lib.hpp"

TEST_CASE("Name is Moradin-Forms", "[library]")
{
  library lib;
  REQUIRE(lib.name == "Moradin-Forms");
}
