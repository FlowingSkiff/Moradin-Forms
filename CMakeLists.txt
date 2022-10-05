cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    Moradin-Forms
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    Moradin-Forms_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    Moradin-Forms_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(Moradin-Forms_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
target_link_libraries(Moradin-Forms_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(Moradin-Forms_exe source/main.cpp)
add_executable(Moradin-Forms::exe ALIAS Moradin-Forms_exe)

set_target_properties(
    Moradin-Forms_exe PROPERTIES
    OUTPUT_NAME Moradin-Forms
    EXPORT_NAME exe
)

target_compile_features(Moradin-Forms_exe PRIVATE cxx_std_20)

target_link_libraries(Moradin-Forms_exe PRIVATE Moradin-Forms_lib)

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
