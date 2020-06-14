cmake_minimum_required(VERSION 3.14)
project(BoostAsioTest)

find_package(Threads REQUIRED)

set(CMAKE_BUILD_TYPE Release)

include(../common/ConanHelper.cmake)

add_executable(server)
target_sources(server PRIVATE src/server/main.cpp)
target_compile_features(server PRIVATE cxx_std_20)
target_link_libraries(server
    Threads::Threads
    ${CONAN_LIBS_BOOST_ASIO}
)

add_executable(client)
target_sources(client PRIVATE src/client/main.cpp)
target_compile_features(client PRIVATE cxx_std_20)
target_link_libraries(client
    Threads::Threads
    ${CONAN_LIBS_BOOST_ASIO}
)
