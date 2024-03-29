cmake_minimum_required(VERSION 3.13)
project(mmCEsim-log)

if (USE_THREADS)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

set(CLIP_TESTS             OFF CACHE BOOL "disable clip tests")
set(CLIP_EXAMPLES          OFF CACHE BOOL "disable clip examples")
include_directories(../../ext/clip)
add_subdirectory(${MMCESIM_ROOT_SOURCE_DIR}/ext/clip ${MMCESIM_ROOT_SOURCE_DIR}/bin)

add_executable(mmcesim-log "log.cpp")

target_link_libraries(mmcesim-log LINK_PUBLIC ${Boost_LIBRARIES} clip)
if (USE_THREADS)
    target_link_libraries(mmcesim-log PRIVATE Threads::Threads)
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
    install(TARGETS mmcesim-log DESTINATION "${CMAKE_INSTALL_PREFIX}\\mmCEsim\\bin")
elseif(APPLE)
    # install to the default /usr/local/bin because of SIP restrictions
    install(TARGETS mmcesim-log DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
else()
    # change default to /usr/bin, the same as package installs
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/usr")
    endif()
    install(TARGETS mmcesim-log DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()

if (MMCESIM_TESTS)
    add_test(NAME log_version COMMAND mmcesim-log -v)
    add_test(NAME log_plain   COMMAND mmcesim-log)
    add_test(NAME log_print   COMMAND mmcesim-log -p)
    add_test(NAME log_copy    COMMAND mmcesim-log -c)
    add_test(NAME log_file    COMMAND mmcesim-log -f)
    add_test(NAME log_pc      COMMAND mmcesim-log -pc) # print + copy
endif()
