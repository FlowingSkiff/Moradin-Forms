add_executable(tracer main.cpp)

set_target_properties(
    tracer PROPERTIES
    OUTPUT_NAME Tracer
    EXPORT_NAME exe
)

target_compile_features(
    tracer PRIVATE cxx_std_20
)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(tinyxml2 REQUIRED)

target_link_libraries(
    tracer
    PRIVATE
    fmt::fmt
    spdlog::spdlog
    tinyxml2::tinyxml2
)