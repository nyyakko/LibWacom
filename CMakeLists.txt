cmake_minimum_required(VERSION 3.25)

project(LibWacom LANGUAGES CXX)

include(cmake/get_cpm.cmake)
include(cmake/enable_tests.cmake)
include(GNUInstallDirs)

if (ENABLE_TESTING)
    enable_tests(${PROJECT_NAME})
endif()

CPMAddPackage("gh:fmtlib/fmt#10.2.1")

CPMFindPackage(
    NAME expected
    GITHUB_REPOSITORY nyyakko/expected
    GIT_TAG master
)

CPMFindPackage(
    NAME LibError
    GITHUB_REPOSITORY nyyakko/LibError
    GIT_TAG master
)

CPMFindPackage(
    NAME LibEnum
    GITHUB_REPOSITORY nyyakko/LibEnum
    GIT_TAG master
)

include(cmake/static_analyzers.cmake)

set(LibWacom_CompilerOptions ${LibWacom_CompilerOptions} -Wno-gnu-statement-expression-from-macro-expansion)
# set(LibWacom_LinkerOptions ${LibWacom_LinkerOptions})

set(LibWacom_ExternalLibraries
    fmt::fmt
    LibError::LibError
    LibEnum::LibEnum
)

add_subdirectory(LibWacom)

