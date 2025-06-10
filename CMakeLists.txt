cmake_minimum_required(VERSION 3.25)

project(LibWacom LANGUAGES CXX)

include(cmake/get_cpm.cmake)
include(GNUInstallDirs)

CPMAddPackage(URI "gh:fmtlib/fmt#10.2.1")
CPMAddPackage(URI "gh:nyyakko/expected#master")
CPMAddPackage(URI "gh:nyyakko/LibError#master" OPTIONS "ENABLE_CLANGTIDY OFF" "ENABLE_CPPCHECK OFF" "ENABLE_TESTING OFF")
CPMAddPackage(URI "gh:nyyakko/LibEnum#master"  OPTIONS "ENABLE_CLANGTIDY OFF" "ENABLE_CPPCHECK OFF" "ENABLE_TESTING OFF")

include(cmake/static_analyzers.cmake)
include(cmake/enable_tests.cmake)

if (ENABLE_TESTING)
    enable_tests(${PROJECT_NAME})
endif()

set(LibWacom_CompilerOptions ${LibWacom_CompilerOptions} -Wno-gnu-statement-expression-from-macro-expansion)
# set(LibWacom_LinkerOptions ${LibWacom_LinkerOptions})

set(LibWacom_ExternalLibraries
    fmt::fmt
    LibError::LibError
    LibEnum::LibEnum
)

add_subdirectory(LibWacom)

