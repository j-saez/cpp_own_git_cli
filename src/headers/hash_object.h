#ifndef HASH_OBJECT_H
#define HASH_OBJECT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

void hash_object(int argc, char* argv[]);
void compute_git_hash(std::filesystem::path& filepath, int filetype);
std::string read_blob_file(const std::filesystem::path& filepath);
std::vector<unsigned char> string_to_vector(const std::string& str);
std::vector<unsigned char> compress_data(const std::vector<unsigned char>& input_data);
std::string compute_sh1_hash(std::string filecontent);

#endif // HASH_OBJECT_H
