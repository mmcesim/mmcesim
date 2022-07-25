cmake_minimum_required(VERSION 3.0.0)
project(mmCEsim VERSION 0.1.0)

option(MMCESIM_BUILD_GUI "Build mmCEsim GUI App with Qt" OFF)

# set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_WARN_NEW_VERSIONS ON)
if (UNIX AND NOT APPLE) # Linux
    find_package(
        Boost 1.70 REQUIRED
        COMPONENTS iostreams program_options
    )
else()
    find_package(
        Boost 1.70 REQUIRED
        COMPONENTS iostreams program_options filesystem
    )
endif()

if (UNIX)
    set(USE_THREADS TRUE BOOL "Use Threads in Unix")
else()
    set(USE_THREADS FALSE BOOL "Do not use Threads in Windows")
endif()

if (USE_THREADS)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")

add_subdirectory(ext/yaml-cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # work around for issue with Boost library
    # which is shown https://github.com/bitcoin/bitcoin/pull/24415
    set(CMAKE_CXX_FLAGS -Wno-error=narrowing)
endif()

add_subdirectory(ext/astyle)

include(test/test.cmake)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(mmcesim ${SOURCES})

target_link_libraries(mmcesim LINK_PUBLIC ${Boost_LIBRARIES} yaml-cpp)
if (USE_THREADS)
    target_link_libraries(mmcesim PRIVATE Threads::Threads)
endif()

if (MMCESIM_BUILD_GUI)
    add_subdirectory(gui)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
