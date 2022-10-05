file(
    RELATIVE_PATH relative_path
    "/${Moradin-Forms_INSTALL_CMAKEDIR}"
    "/${CMAKE_INSTALL_BINDIR}/${Moradin-Forms_NAME}"
)

get_filename_component(prefix "${CMAKE_INSTALL_PREFIX}" ABSOLUTE)
set(config_dir "${prefix}/${Moradin-Forms_INSTALL_CMAKEDIR}")
set(config_file "${config_dir}/Moradin-FormsConfig.cmake")

message(STATUS "Installing: ${config_file}")
file(WRITE "${config_file}" "\
get_filename_component(
    _Moradin-Forms_executable
    \"\${CMAKE_CURRENT_LIST_DIR}/${relative_path}\"
    ABSOLUTE
)
set(
    MORADIN_FORMS_EXECUTABLE \"\${_Moradin-Forms_executable}\"
    CACHE FILEPATH \"Path to the Moradin-Forms executable\"
)
")
list(APPEND CMAKE_INSTALL_MANIFEST_FILES "${config_file}")
