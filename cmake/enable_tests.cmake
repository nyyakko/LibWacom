function(enable_tests PROJECT)
    include(GoogleTest)

    if (NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        message(STATUS "[${PROJECT}] running on ${CMAKE_HOST_SYSTEM_NAME}, sanitizers for tests are enabled.")
        set(LibWacom_TestsLinkerOptions ${LibWacom_TestsLinkerOptions} -fsanitize=undefined,leak,address)
    else()
        message(STATUS "[${PROJECT}] running on ${CMAKE_HOST_SYSTEM_NAME}, sanitizers for tests are disabled.")
    endif()

    set(LibWacom_TestsCompilerOptions ${LibWacom_TestsCompilerOptions} ${LibWacom_CompilerOptions})
    set(LibWacom_TestsLinkerOptions ${LibWacom_TestsLinkerOptions} ${LibWacom_LinkerOptions})

    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG release-1.12.1
        VERSION 1.12.1
        OPTIONS
            "INSTALL_GTEST OFF"
            "gtest_force_shared_crt"
    )

    enable_testing()
    add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endfunction()
