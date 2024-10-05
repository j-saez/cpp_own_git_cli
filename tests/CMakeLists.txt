# tests/CMakeLists.test.txt
# Set up for Google Test (for testing purposes)
enable_testing()

# Download and include Google Test (this will fetch Google Test automatically)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add an executable for your tests
add_executable(run_tests
    tests/test_git_init.cpp
    tests/test_cat_file.cpp
    tests/test_hash_object.cpp

    # Include the source files needed for testing
    ../src/git_init.cpp
    ../src/cat_file.cpp
    ../src/hash_object.cpp
)

# Include directories for your test files as well
target_include_directories(run_tests PRIVATE ../src/headers)

# Link GoogleTest to your test executable
target_link_libraries(run_tests PRIVATE gtest gtest_main)

# Link OpenSSL to your test executable
target_link_libraries(run_tests PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Link Zlib to your test executable
target_link_libraries(run_tests PRIVATE ZLIB::ZLIB)

# Register the tests
add_test(NAME run_tests COMMAND run_tests)

