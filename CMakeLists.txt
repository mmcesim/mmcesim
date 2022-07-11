cmake_minimum_required(VERSION 3.0.0)
project(mmCEsim VERSION 0.1.0)

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
    set(USE_THREADS true BOOL "Use Threads in Unix")
else()
    set(USE_THREADS false BOOL "Do not use Threads in Windows")
endif()

if (USE_THREADS)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
endif()

find_package(yaml-cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
    ${YAML_CPP_INCLUDE_DIRS}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# work around for issue with Boost library
# which is shown https://github.com/bitcoin/bitcoin/pull/24415
if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS -Wno-error=narrowing)
endif()

add_subdirectory(ext/astyle)

include(CTest)
enable_testing()
add_test(NAME astyle COMMAND astyle -h)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(mmcesim ${SOURCES})

target_link_libraries(mmcesim LINK_PUBLIC ${Boost_LIBRARIES} ${YAML_CPP_LIBRARIES})
if (USE_THREADS)
    target_link_libraries(mmcesim PRIVATE Threads::Threads)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
