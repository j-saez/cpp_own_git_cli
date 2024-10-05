#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "../src/headers/git_init.h"

class GitInitTest : public ::testing::Test {
protected:
    std::filesystem::path test_dir;

    void SetUp() override {
        // Create a temporary directory for testing
        test_dir = std::filesystem::temp_directory_path() / "git_test_repo";
        std::filesystem::create_directory(test_dir);
    }

    void TearDown() override {
        // Clean up the temporary directory after each test
        if (std::filesystem::exists(test_dir)) {
            std::filesystem::remove_all(test_dir);
        }
    }
};

// Test git_init success case
TEST_F(GitInitTest, GitInitSuccess) {
    char* argv[] = { (char*)"git_init", (char*)"-i", (char*)test_dir.c_str() };
    int argc = 3;
    EXPECT_EQ(git_init(argc, argv), 0);
    EXPECT_TRUE(std::filesystem::exists(test_dir / ".git"));
    EXPECT_TRUE(std::filesystem::exists(test_dir / ".git/objects"));
    EXPECT_TRUE(std::filesystem::exists(test_dir / ".git/refs"));
    EXPECT_TRUE(std::filesystem::exists(test_dir / ".git/HEAD"));
}

// Test git_init failure when path doesn't exist
TEST_F(GitInitTest, GitInitPathNotExist) {
    std::filesystem::path non_existent_dir = test_dir / "non_existent";
    char* argv[] = { (char*)"git_init", (char*)"-i", (char*)non_existent_dir.c_str() };
    int argc = 3;
    EXPECT_EQ(git_init(argc, argv), -1);
}

// Test git_init failure when .git directory already exists
TEST_F(GitInitTest, GitInitGitAlreadyExists) {
    std::filesystem::create_directory(test_dir / ".git");  // Pre-create a .git directory
    char* argv[] = { (char*)"git_init", (char*)"-i", (char*)test_dir.c_str() };
    int argc = 3;
    EXPECT_EQ(git_init(argc, argv), -1);  // Should fail because .git already exists
}

// Test create_git_init_dirs success case
TEST_F(GitInitTest, CreateGitInitDirsSuccess) {
    std::filesystem::path git_dir = test_dir / ".git";
    EXPECT_EQ(create_git_init_dirs(git_dir), 0);
    EXPECT_TRUE(std::filesystem::exists(git_dir));
    EXPECT_TRUE(std::filesystem::exists(git_dir / "objects"));
    EXPECT_TRUE(std::filesystem::exists(git_dir / "refs"));
}

// Test create_git_init_dirs failure when .git already exists
TEST_F(GitInitTest, CreateGitInitDirsAlreadyExists) {
    std::filesystem::path git_dir = test_dir / ".git";
    std::filesystem::create_directory(git_dir);  // Pre-create a .git directory
    EXPECT_EQ(create_git_init_dirs(git_dir), -1);  // Should fail because .git already exists
}

// Test create_head_file success case
TEST_F(GitInitTest, CreateHeadFileSuccess) {
    std::filesystem::path git_dir = test_dir / ".git";
    std::filesystem::create_directory(git_dir);
    std::ofstream head_file(git_dir / "HEAD");
    EXPECT_EQ(create_head_file(head_file, git_dir), 0);
    EXPECT_TRUE(std::filesystem::exists(git_dir / "HEAD"));

    std::ifstream file(git_dir / "HEAD");
    std::string content;
    std::getline(file, content);
    EXPECT_EQ(content, "ref: refs/heads/master");
}

// Test create_head_file failure case
TEST_F(GitInitTest, CreateHeadFileFailure) {
    std::filesystem::path git_dir = test_dir / ".git";
    std::ofstream head_file("/invalid_path/HEAD");  // Invalid file path
    EXPECT_EQ(create_head_file(head_file, git_dir), -1);  // Should fail
}

