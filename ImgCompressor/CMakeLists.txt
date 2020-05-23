cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

project(ImgCompressor)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.15/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan/conan.cmake)
conan_cmake_run(
  BASIC_SETUP
  NO_OUTPUT_DIRS
  INSTALL_FOLDER ${CMAKE_BINARY_DIR}/conan
  CONANFILE conanfile.txt
  BUILD missing
  INSTALL_ARGS -s build_type=${CMAKE_BUILD_TYPE}
)

add_executable(app)
target_sources(app PRIVATE src/main.cpp)
target_compile_features(app PRIVATE cxx_std_20)
target_link_libraries(app
    ${CONAN_LIBS_SFML}
    ${CONAN_LIBS_BOOST_RANGE}
    ${CONAN_LIBS_DOCOPT.CPP}
)

set_target_properties(app PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
