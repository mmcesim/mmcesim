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

if(WIN32)
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(pf86 "PROGRAMFILES(x86)")
        set(prog_files $ENV{${pf86}})
        if (NOT ${prog_files})
            set(prog_files $ENV{PROGRAMFILES})
        endif()
        set(CMAKE_INSTALL_PREFIX prog_files)
    endif()
    install(TARGETS mmcesim-maintain DESTINATION "${CMAKE_INSTALL_PREFIX}\\mmCEsim\\bin")
elseif(APPLE)
    # install to the default /usr/local/bin because of SIP restrictions
    install(TARGETS mmcesim-maintain DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
else()
    # change default to /usr/bin, the same as package installs
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/usr")
    endif()
    install(TARGETS mmcesim-maintain DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()

if (MMCESIM_TESTS)
    add_test(NAME maintain_l COMMAND mmcesim-maintain -l)
endif()
