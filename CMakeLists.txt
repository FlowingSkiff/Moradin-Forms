cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)
cmake_policy(SET CMP0091 NEW)
project(
  Moradin-Forms
  VERSION 0.1.0
  DESCRIPTION "Short description"
  HOMEPAGE_URL "https://example.com/"
  LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

add_subdirectory(source)

# ---- Install rules ----
if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----
if(NOT Moradin-Forms_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
    AUTHOR_WARNING
    "Developer mode is intended for developers of Moradin-Forms"
  )
endif()

include(cmake/dev-mode.cmake)
