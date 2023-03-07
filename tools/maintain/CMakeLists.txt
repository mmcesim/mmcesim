cmake_minimum_required(VERSION 3.13)
project(mmCEsim-maintain)

add_executable(mmcesim-maintain "maintain.cpp")

target_link_libraries(mmcesim-maintain LINK_PUBLIC ${Boost_LIBRARIES})

add_test(NAME maintain_l COMMAND mmcesim-maintain -l)
