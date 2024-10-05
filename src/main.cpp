#include <iostream>
#include <string>
#include "headers/git_init.h"
#include "headers/cat_file.h"
#include "headers/hash_object.h"
#include "headers/list_tree.h"

int main(int argc, char* argv[]) {
    if (argc == 1) return 0;

    std::string command = argv[1];
    if (command == "init"){ if (git_init(argc, argv) == -1) return -1;}
    else if (command == "cat-file") cat_file(argc, argv);
    else if (command == "hash-object") hash_object(argc, argv);
    else if (command == "ls-tree") list_tree(argc, argv);
    else{
        std::cerr << "Error: Please run one of the available commands: init, cat-file, hash-object, ls-tree" << std::endl;
        return -1;  // Exit the function if no path is provided
    }

    return 0;
}
