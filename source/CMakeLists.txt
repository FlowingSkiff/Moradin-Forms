add_subdirectory(Core)
add_subdirectory(Utility)
add_executable(Moradin-Forms_exe main.cpp)
add_executable(Moradin-Forms::exe ALIAS Moradin-Forms_exe)

set_target_properties(
    Moradin-Forms_exe PROPERTIES
    OUTPUT_NAME Moradin-Forms
    EXPORT_NAME exe
)

target_compile_features(
    Moradin-Forms_exe PRIVATE cxx_std_20
)

find_package(spdlog REQUIRED)
find_package(tinyxml2 REQUIRED)

target_link_libraries(
    Moradin-Forms_exe
    PRIVATE
    Core_lib
    spdlog::spdlog
    tinyxml2::tinyxml2
)

target_include_directories(
    Moradin-Forms_exe ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)