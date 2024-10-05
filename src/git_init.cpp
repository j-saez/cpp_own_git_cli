#include <fstream>
#include "headers/git_init.h"

int git_init(int argc, char* argv[]) {
    // Check if the user has provided a path
    if (argc <= 2) {
        std::cerr << "Error: Please specify the path to initialize the Git directory." << std::endl;
        return -1;  // Exit the function if no path is provided
    }

    // Get the path from the command line argument
    std::filesystem::path init_path = argv[2];
    if (!std::filesystem::exists(init_path)) {
        std::cerr << "Error: The specified path does not exist." << std::endl;
        return -1;
    }

    std::filesystem::path git_dir = init_path / ".git";
    if (create_git_init_dirs(git_dir) == -1) return -1;

    std::ofstream head_file(git_dir / "HEAD");
    if (create_head_file(head_file, git_dir) == -1) return -1;

    std::cout << "Git directory initialized at: " << git_dir << std::endl;
    return 0;
}

int create_git_init_dirs(std::filesystem::path &git_dir) {
    if (std::filesystem::exists(git_dir)){
        std::cerr << "The .git repository you are trying to create already exists." << std::endl;
        return -1;
    }

    if (!std::filesystem::create_directory(git_dir)) {
        std::cerr << "Error: Could not create .git directory." << std::endl;
        return -1;
    }
    if (!std::filesystem::create_directory(git_dir / "objects")) {
        std::cerr << "Error: Could not create objects directory." << std::endl;
        return -1;
    }
    if (!std::filesystem::create_directory(git_dir / "refs")) {
        std::cerr << "Error: Could not create refs directory." << std::endl;
        return -1;
    }
    return 0;
}

int create_head_file(std::ofstream &head_file, std::filesystem::path &git_dir) {
    if (!head_file.is_open()) {
        std::cerr << "Error: Unable to create HEAD file. Cleaning up..." << std::endl;
        std::filesystem::remove_all(git_dir);  // Clean up partially created .git directory
        return -1;
    }
    head_file << "ref: refs/heads/master\n";
    head_file.close();
    return 0;
}
