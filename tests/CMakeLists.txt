FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

add_executable(hyperslab-test hyperslab-test.cpp)
target_link_libraries(hyperslab-test PRIVATE gtest gtest_main ggcmiw PkgConfig::NETCDF) 

add_executable(location-test location-test.cpp)
target_link_libraries(location-test PRIVATE gtest gtest_main ggcmiw)

add_executable(calendar-test  calendar-test.cpp)
target_link_libraries(calendar-test PRIVATE gtest gtest_main ggcmiw)

add_executable(config-test config-test.cpp)
target_link_libraries(config-test PRIVATE gtest gtest_main ggcmiw PkgConfig::JANSSON)

add_test(NAME test-hyperslab COMMAND hyperslab-test)
add_test(NAME test-location COMMAND location-test)
add_test(NAME test-calendar COMMAND calendar-test)
add_test(NAME test-config COMMAND config-test)