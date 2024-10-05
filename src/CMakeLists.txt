# Add the executable for the main project
add_executable(
    own_git
    main.cpp
    cat_file.cpp
    git_init.cpp
    hash_object.cpp
    list_tree.cpp
    helpers.cpp
)

# Include directories for your headers
target_include_directories(own_git PRIVATE headers)

# Find and link OpenSSL libraries (for SHA-1 hash)
find_package(OpenSSL REQUIRED)
target_link_libraries(own_git PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Find and link Zlib (for -lz)
find_package(ZLIB REQUIRED)
target_link_libraries(own_git PRIVATE ZLIB::ZLIB)
