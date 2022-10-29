#include <iostream>
#include <string>
#include <utility>

#include <spdlog/spdlog.h>
#include <tinyxml2.h>

#include "Core/lib.hpp"

#ifndef AURORA_PATH
// #  error "AURORA_PATH is not defined"
#endif

auto main() -> int
{
  library lib;
  std::string message = "Hello from " + lib.name + "!";
  std::cout << message << '\n';

  return 0;
}
