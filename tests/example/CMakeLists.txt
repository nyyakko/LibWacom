set(TEST_NAME example)

project(${TEST_NAME} LANGUAGES CXX)

add_executable(${TEST_NAME} Main.cpp)

enable_clang_tidy(${TEST_NAME})
enable_cppcheck(${TEST_NAME})

target_compile_features(${TEST_NAME} PRIVATE cxx_std_23)

target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest_main LibWacom)
target_compile_options(${TEST_NAME} PRIVATE ${LibWacom_TestsCompilerOptions})
target_link_options(${TEST_NAME} PRIVATE ${LibWacom_TestsLinkerOptions})

gtest_discover_tests(${TEST_NAME})
