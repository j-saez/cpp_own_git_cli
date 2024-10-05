#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <filesystem>
#include <vector>

std::vector<unsigned char> read_git_object(const std::filesystem::path& filepath);
std::vector<unsigned char> decompress_git_object(const std::vector<unsigned char>& compressed_data);
std::tuple<std::string, size_t, size_t> get_git_object_type_and_size(const std::vector<unsigned char>& decompressed_data);
void print_git_object(const std::vector<unsigned char>& decompressed_data);

#endif // HELPERS_H
