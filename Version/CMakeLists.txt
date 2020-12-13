cmake_minimum_required(VERSION 3.14)
project(SqliteTest)

set(CMAKE_BUILD_TYPE Release)

include(../common/ConanHelper.cmake)

set(VER $ENV{VER})
if(NOT "${VER}")
    set(VER 1)
endif()

add_executable(app)
target_sources(app PRIVATE src/main.cpp src/Object.cpp src/impl/v${VER}/ImplObject.cpp)
target_compile_features(app PRIVATE cxx_std_20)
target_include_directories(app PRIVATE src)
target_compile_definitions(app PRIVATE VER=${VER})
