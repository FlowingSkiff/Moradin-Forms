set(
    core_sources
    lib.cpp
)

add_library(
    Core_lib OBJECT
    ${core_sources}
)

target_include_directories(
    Core_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(
    Core_lib PUBLIC cxx_std_20
)

find_package(fmt REQUIRED)
target_link_libraries(Core_lib PRIVATE fmt::fmt)