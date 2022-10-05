include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package Moradin-Forms)

install(
    TARGETS Moradin-Forms_exe
    RUNTIME COMPONENT Moradin-Forms_Runtime
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    Moradin-Forms_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(Moradin-Forms_INSTALL_CMAKEDIR)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${Moradin-Forms_INSTALL_CMAKEDIR}"
    COMPONENT Moradin-Forms_Development
)

# Export variables for the install script to use
install(CODE "
set(Moradin-Forms_NAME [[$<TARGET_FILE_NAME:Moradin-Forms_exe>]])
set(Moradin-Forms_INSTALL_CMAKEDIR [[${Moradin-Forms_INSTALL_CMAKEDIR}]])
set(CMAKE_INSTALL_BINDIR [[${CMAKE_INSTALL_BINDIR}]])
" COMPONENT Moradin-Forms_Development)

install(
    SCRIPT cmake/install-script.cmake
    COMPONENT Moradin-Forms_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
