cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(vn_driver)

# this is required since the generated vectornav library doesnt use the find_dependency macro in a cmake.in file ... TODO fix upstream
find_package(LibComms REQUIRED)

find_package(vectornav REQUIRED)

add_executable(vn_driver
    src/main.cpp
)

target_link_libraries(vn_driver PUBLIC cc::vectornav)
set_target_properties(vn_driver PROPERTIES CXX_STANDARD 17)
target_compile_features(vn_driver PRIVATE cxx_std_17)

install(TARGETS vn_driver DESTINATION bin)