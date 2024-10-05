#ifndef GIT_INIT_H
#define GIT_INIT_H

#include <iostream>
#include <filesystem>

int git_init(int argc, char* argv[]);
int create_git_init_dirs(std::filesystem::path &git_dir);
int create_head_file(std::ofstream &head_file, std::filesystem::path &git_dir);

#endif // GIT_INIT_H
