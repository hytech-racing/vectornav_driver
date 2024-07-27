cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(vn_driver)

find_package(cmake_macros)

include(create_package)
include(easy_find_packages)


easy_find_packages(vn_driver
    "LibComms"
    "vectornav"
    "Boost system"
)

add_library(vn_driver SHARED src/driver.cpp)

target_link_libraries(vn_driver PUBLIC cc::vectornav Boost::system)
set_target_properties(vn_driver PROPERTIES CXX_STANDARD 17)
target_compile_features(vn_driver PRIVATE cxx_std_17)
target_include_directories(vn_driver
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/include>"
)

include(create_package)
create_package(vn_driver vn_driver)
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    FILES_MATCHING PATTERN "*.h" 
    )

# tests 
add_executable(vn_driver_cons_test
    test/main_cons.cpp
)

add_executable(vn_driver_tester
    test/main_prod.cpp
)

target_link_libraries(vn_driver_cons_test PUBLIC vn_driver)
target_link_libraries(vn_driver_tester PUBLIC vn_driver)


set_target_properties(vn_driver_tester PROPERTIES CXX_STANDARD 17)
target_compile_features(vn_driver_tester PRIVATE cxx_std_17)

install(TARGETS vn_driver_cons_test vn_driver_tester DESTINATION bin)