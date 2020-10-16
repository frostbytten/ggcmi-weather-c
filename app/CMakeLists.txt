add_executable(ggcmi2dssatw main.c)
target_include_directories(ggcmi2dssatw PRIVATE ${MPI_C_INCLUDE_PATH})
target_link_libraries(ggcmi2dssatw PRIVATE MPI::MPI_C ggcmiw)
