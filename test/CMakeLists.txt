# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(Moradin-FormsTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(Moradin-Forms_test source/Moradin-Forms_test.cpp)
target_link_libraries(
    Moradin-Forms_test PRIVATE
    Moradin-Forms_lib
    Catch2::Catch2WithMain
)
target_compile_features(Moradin-Forms_test PRIVATE cxx_std_20)

catch_discover_tests(Moradin-Forms_test)

# ---- End-of-file commands ----

add_folders(Test)
