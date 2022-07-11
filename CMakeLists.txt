cmake_minimum_required(VERSION 3.0.0)
project(mmCEsim VERSION 0.1.0)

set(Boost_USE_STATIC_LIBS ON)
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

find_package(yaml-cpp 0.5 REQUIRED)
# https://stackoverflow.com/a/62779940/15080514
message(STATUS "YAML-CPP config file: ${yaml-cpp_CONFIG}")

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
    ${YAML_CPP_INCLUDE_DIR}
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
else()
    # for yaml-cpp static linking
    # reference: https://stackoverflow.com/q/14172856/15080514
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
endif()

add_subdirectory(ext/astyle)

include(CTest)
enable_testing()
add_test(NAME astyle   COMMAND astyle -h)
add_test(NAME help     COMMAND mmcesim -h)
add_test(NAME version  COMMAND mmcesim --version)
add_test(NAME null1    COMMAND mmcesim) # [will fail]
add_test(NAME null2    COMMAND mmcesim sim) # [will fail]
add_test(NAME basic    COMMAND mmcesim sim ../test/MIMO.sim)
add_test(NAME notexist COMMAND mmcesim sim input_not_exists) # [will fail]
set_tests_properties(null1 null2 notexist PROPERTIES WILL_FAIL TRUE)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(mmcesim ${SOURCES})

target_link_libraries(mmcesim LINK_PUBLIC ${Boost_LIBRARIES} ${YAML_CPP_LIBRARIES})
if (USE_THREADS)
    target_link_libraries(mmcesim PRIVATE Threads::Threads)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
