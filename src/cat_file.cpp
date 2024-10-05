#include "headers/cat_file.h"
#include "headers/helpers.h"
#include <ostream>
#include <vector>
#include <zlib.h>

void cat_file(int argc, char* argv[]) {
    // Check if the user has provided a path
    if (argc != 4) {
        std::cerr << "Error. Usage: ./own_git cat-file -p <hash value>" << std::endl;
        return;  // Exit the function if no path is provided
    }

    std::string option = argv[2];
    if (option != "-p") {
        std::cerr << "Error. The only available option is -p. Usage: ./own_git cat-file -p <hash value>" << std::endl;
        return;
    }

    std::string hash = argv[3];
    std::string object_dir = hash.substr(0,2);
    std::string filename = hash.substr(2);

    std::filesystem::path filepath = std::filesystem::path(".git/objects") / object_dir / filename;
    std::vector<unsigned char> decompressed_data = decompress_git_object(read_git_object(filepath));
    print_git_object(decompressed_data);

    return;
}
