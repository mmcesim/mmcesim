cmake_minimum_required(VERSION 3.13)
project(mmCEsim-maintain)

if (USE_THREADS)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

add_executable(mmcesim-maintain "maintain.cpp")

target_link_libraries(mmcesim-maintain LINK_PUBLIC ${Boost_LIBRARIES})
if (USE_THREADS)
    target_link_libraries(mmcesim-maintain PRIVATE Threads::Threads)
endif()

add_test(NAME maintain_l COMMAND mmcesim-maintain -l)
