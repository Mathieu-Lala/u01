cmake_minimum_required(VERSION 3.14)
project(ImgCompressor)

set(CMAKE_BUILD_TYPE Release)

include(../common/ConanHelper.cmake)

add_executable(app)
target_sources(app PRIVATE src/main.cpp)
target_compile_features(app PRIVATE cxx_std_20)
target_link_libraries(app
    ${CONAN_LIBS_SFML}
    ${CONAN_LIBS_BOOST_RANGE}
    ${CONAN_LIBS_DOCOPT.CPP}
)
