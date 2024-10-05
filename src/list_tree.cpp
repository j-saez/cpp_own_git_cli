#include "headers/helpers.h"
#include "headers/cat_file.h"
#include <ostream>
#include "headers/list_tree.h"

void list_tree(int argc, char* argv[]) {
    // Check if the user has provided a path
    if (argc != 4) {
        std::cerr << "Error. Usage: ./own_git ls-tree --name-only <tree_sha>" << std::endl;
        return;  // Exit the function if no path is provided
    }

    std::string option = argv[2];
    if (option != "--name-only") {
        std::cerr << "Error. The only available option is --name-only. Usage: ./own_git ls-tree --name-only <tree_hash>" << std::endl;
        return;
    }

    std::string tree_hash = argv[3];
    std::string object_dir = tree_hash.substr(0,2);
    std::string filename = tree_hash.substr(2);

    std::filesystem::path filepath = std::filesystem::path(".git/objects") / object_dir / filename;
    std::vector<unsigned char> decompressed_data = decompress_git_object(read_git_object(filepath));

    auto [type,size,index] = get_git_object_type_and_size(decompressed_data);
    if (type != "tree") {
        std::cerr << "Error. Not an object tree." << std::endl;
        return;
    }


    while (index < decompressed_data.size()) {
        std::string mode;
        while (index < decompressed_data.size() && decompressed_data[index] != ' ') {
            mode += static_cast<char>(decompressed_data[index]);
            index++;
        }
        index++;

        std::string filename;
        while (index < decompressed_data.size() && decompressed_data[index] != '\0') {
            filename += static_cast<char>(decompressed_data[index]);
            index++;
        }
        index++;

        index += 20; // Skip SHA-1 hash

        std::cout << filename << "\n";
    }

    return;
}

