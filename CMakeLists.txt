cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(vn_driver)

# this is required since the generated vectornav library doesnt use the find_dependency macro in a cmake.in file ... TODO fix upstream
find_package(LibComms REQUIRED)

find_package(vectornav REQUIRED)
find_package(Boost REQUIRED system)
add_executable(vn_driver
    src/main_cons.cpp
    src/driver.cpp
)

add_executable(vn_driver_tester
    src/main_prod.cpp
    src/driver.cpp
)

target_link_libraries(vn_driver PUBLIC cc::vectornav Boost::system)
set_target_properties(vn_driver PROPERTIES CXX_STANDARD 17)
target_compile_features(vn_driver PRIVATE cxx_std_17)
target_include_directories(vn_driver
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
)

install(TARGETS vn_driver DESTINATION bin)


target_link_libraries(vn_driver_tester PUBLIC cc::vectornav Boost::system)
set_target_properties(vn_driver_tester PROPERTIES CXX_STANDARD 17)
target_compile_features(vn_driver_tester PRIVATE cxx_std_17)
target_include_directories(vn_driver_tester
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
)

install(TARGETS vn_driver_tester DESTINATION bin)