cmake_minimum_required(VERSION 3.13)
project(mmCEsim VERSION 0.2.1)

option(MMCESIM_BUILD_ASTYLE   "Build astyle Code Formatter"             ON )
option(MMCESIM_BUILD_COMPOSE  "Build composition tool"                  ON )
option(MMCESIM_BUILD_LOG      "Build log tool"                          ON )
option(MMCESIM_BUILD_MAINTAIN "Build mantainence tool"                  ON )
option(MMCESIM_BUILD_GUI      "Build mmCEsim GUI App with Qt"           OFF)
option(MMCESIM_APPLE_COPY_SH  "Copy additional shell script for macOS"  OFF)
option(MMCESIM_TESTS          "Run mmCEsim tests"                       ON )

set(MMCESIM_ROOT_SOURCE_DIR ${CMAKE_SOURCE_DIR})

# Default as Release
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(Boost_USE_STATIC_LIBS ON)
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

set(YAML_CPP_BUILD_TESTS   OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS   OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
set(CLIP_TESTS             OFF CACHE BOOL "disable clip tests")
set(CLIP_EXAMPLES          OFF CACHE BOOL "disable clip examples")

add_subdirectory(ext/yaml-cpp)
add_subdirectory(ext/fmt)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ext/fmt/include
    ${CMAKE_SOURCE_DIR}/ext/spy/src
    ${Boost_INCLUDE_DIR}
)
link_directories(${Boost_LIBRARY_DIRS})

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

if (MMCESIM_BUILD_ASTYLE)
    add_subdirectory(ext/astyle)
endif()

include(test/test.cmake)

# file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
set(SOURCES
    "src/config.cpp"
    "src/export.cpp"
    "src/format_yaml.cpp"
    "src/log.cpp"
    "src/main.cpp"
    "src/read.cpp"
    "src/simulate.cpp"
    "src/style.cpp"
    "src/term.cpp"
    "src/export/alg_line.cpp"
    "src/export/alg.cpp"
    "src/export/calc.cpp"
    "src/export/channel_graph.cpp"
    "src/export/macro.cpp"
    "src/export/type_track.cpp"
    "src/export/type.cpp"
)
add_executable(mmcesim ${SOURCES})

target_link_libraries(mmcesim LINK_PUBLIC ${Boost_LIBRARIES} yaml-cpp fmt::fmt)
if (USE_THREADS)
    target_link_libraries(mmcesim PRIVATE Threads::Threads)
endif()

if (MMCESIM_BUILD_GUI)
    add_subdirectory(gui)
endif()
if (MMCESIM_BUILD_LOG)
    add_subdirectory(tools/log)
endif()
if (MMCESIM_BUILD_MAINTAIN)
    add_subdirectory(tools/maintain)
endif()

list(APPEND MMCESIM_DOCS
    LICENSE
    README.md
    CONTRIBUTING.md
    CODE_OF_CONDUCT.md
    doc/mmCEsim-doc.pdf
)

if (WIN32)
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(pf86 "PROGRAMFILES(x86)")
        set(prog_files $ENV{${pf86}})
        if (NOT ${prog_files})
            set(prog_files $ENV{PROGRAMFILES})
        endif()
        set(CMAKE_INSTALL_PREFIX prog_files)
    endif()
    install(TARGETS mmcesim DESTINATION "${CMAKE_INSTALL_PREFIX}\\mmCEsim\\bin")
    install(FILES ${MMCESIM_DOCS} DESTINATION "${CMAKE_INSTALL_PREFIX}\\mmCEsim\\doc")
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/mmcesim DESTINATION "${CMAKE_INSTALL_PREFIX}\\mmCEsim\\include\\mmcesim")
elseif (APPLE)
    # install to the default /usr/local/bin because of SIP restrictions
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/usr/local")
    endif()
    install(TARGETS mmcesim DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(FILES ${MMCESIM_DOCS} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/mmcesim")
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/mmcesim DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
        PATTERN ".DS_Store" EXCLUDE
    )
    if (MMCESIM_APPLE_COPY_SH)
        install(PROGRAMS scripts/mac_trust_app.sh DESTINATION "${CMAKE_INSTALL_PREFIX}" RENAME trust_mmcesim.sh)
    endif()
else ()
    # change default to /usr/bin, the same as package installs
    if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/usr")
    endif()
    install(TARGETS mmcesim DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
    install(FILES ${MMCESIM_DOCS} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/mmcesim")
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/mmcesim DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
