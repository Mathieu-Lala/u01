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
  INSTALL_ARGS -s build_type=Release
)
