cmake_minimum_required(VERSION 3.25)

project(LibWacom LANGUAGES CXX)

include(cmake/get_cpm.cmake)
include(GNUInstallDirs)

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage("gh:nyyakko/expected#master")
CPMAddPackage("gh:nyyakko/LibError#master")
CPMAddPackage("gh:nyyakko/LibEnum#master")

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

