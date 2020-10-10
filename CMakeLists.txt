cmake_minimum_required(VERSION 3.12..3.18)
project(ggcmi_wth_gen VERSION 1.0.0
                      DESCRIPTION "Weather file aggregator from ISIMIP Weather to DSSAT")

#set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -fno-omit-frame-pointer -fsanitize=address")

include(FetchContent)
find_package(MPI REQUIRED)
find_package(PkgConfig REQUIRED)


pkg_check_modules(JANSSON REQUIRED IMPORTED_TARGET jansson)
pkg_check_modules(NETCDF REQUIRED IMPORTED_TARGET netcdf)
pkg_check_modules(UDUNITS REQUIRED IMPORTED_TARGET udunits)
set(SOURCE_FILES
        config.c config.h
        io.c io.h
        location.c location.h
        hyperslab.c hyperslab.h
        main.c unit_util.c unit_util.h calendar.c calendar.h)
add_executable(ggcmi-wth-gen ${SOURCE_FILES})
target_link_libraries(ggcmi-wth-gen PRIVATE PkgConfig::JANSSON PkgConfig::NETCDF PkgConfig::UDUNITS)

# test stuff
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

