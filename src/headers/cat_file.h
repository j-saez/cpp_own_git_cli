#ifndef CAT_FILE_H
#define CAT_FILE_H

#include <iostream>
#include <filesystem>
#include <vector>

void cat_file(int argc, char* argv[]);
std::vector<unsigned char> read_git_object(const std::filesystem::path& filepath);
std::vector<unsigned char> decompress_git_object(const std::vector<unsigned char>& compressed_data);
void print_file_content(const std::vector<unsigned char>& decompressed_data);

#endif // CAT_FILE_H
