#include "headers/hash_object.h"
#include <openssl/sha.h>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <zlib.h>

void hash_object(int argc, char* argv[]) {

    /*
     * TODO
     * */

    if (argc != 4) {
        std::cerr << "Error. Usage: ./own_git hash-object -w <hash value>" << std::endl;
        return;
    }

    std::string option = argv[2];
    if (option != "-w") {
        std::cerr << "Error. The only available option is -w. Usage: ./own_git hash-object -w <filename>" << std::endl;
        return;
    }

    std::filesystem::path filepath = argv[3];
    int filetype = 0; // TODO: Create a functio like git cat-file -t for getting the type of the file.
    compute_git_hash(filepath, filetype);
}

void compute_git_hash(std::filesystem::path& filepath, int filetype) {

    /*
     * TODO
     * */

    std::string filecontent;

    // Assign global variable for filetypes (0 for blobs...)
    if (filetype == 0) {
        filecontent = read_blob_file(filepath);
    }

    if (filecontent.empty()) return; // Handle the empty case if needed

    std::vector<unsigned char> compressed_filecontent = compress_data(string_to_vector(filecontent));
    std::string sh1_hash = compute_sh1_hash(filecontent);

    // Construct the path to the .git/objects directory
    std::filesystem::path git_objects_dir = filepath.parent_path() / ".git" / "objects";
    std::filesystem::path obj_dir = git_objects_dir / sh1_hash.substr(0, 2); // First two characters of the hash
    std::filesystem::path obj_filename = obj_dir / sh1_hash.substr(2); // Remaining part of the hash

    // Create the directory if it doesn't exist
    std::filesystem::create_directories(obj_dir);

    // Write the compressed data to the file
    std::ofstream out_file(obj_filename, std::ios::binary);
    if (!out_file) {
        std::cerr << "Error: Could not create object file at " << obj_filename << std::endl;
        return;
    }

    out_file.write(reinterpret_cast<const char*>(compressed_filecontent.data()), compressed_filecontent.size());
    out_file.close();

    std::cout << sh1_hash << std::endl;
    return;
}

std::string compute_sh1_hash(std::string filecontent) {
    // Create a buffer to store the hash result and Apply the SHA1 hash function
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(filecontent.c_str()), filecontent.size(), hash);

    // Convert the hash to a hexadecimal string
    std::ostringstream hash_oss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        hash_oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return hash_oss.str(); // Return the hash as a hexadecimal string
}

std::string read_blob_file(const std::filesystem::path& filepath) {

    /*
     * TODO
     * */

    std::ifstream file(filepath);
    std::stringstream buffer;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file at " << filepath << std::endl;
        return "";
    }

    // Read the entire file into the stringstream
    buffer << file.rdbuf();
    file.close();

    std::string filecontent = buffer.str();

    // Create the Git header
    std::ostringstream oss;
    oss << "blob " << filecontent.size() << '\0' << filecontent; // Git header

    return oss.str(); // Return the Git-formatted blob content
}

std::vector<unsigned char> string_to_vector(const std::string& str) {
    return std::vector<unsigned char>(str.begin(), str.end());
}

std::vector<unsigned char> compress_data(const std::vector<unsigned char>& input_data) {
    // Set up a zlib stream for compression
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = input_data.size(); // Input size
    stream.next_in = const_cast<Bytef*>(input_data.data());  // Input data

    // Initialize the compression stream
    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
        std::cerr << "Error: Failed to initialize zlib compression" << std::endl;
        return {};
    }

    // Create a buffer to store the compressed data
    std::vector<unsigned char> compressed_data;
    unsigned char out_buffer[8192];  // Temporary buffer for compression

    // Perform the compression in chunks
    int ret;
    do {
        stream.avail_out = sizeof(out_buffer);  // Size of the output buffer
        stream.next_out = out_buffer;  // Output location

        // Compress data
        ret = deflate(&stream, Z_FINISH);  // Finish the compression

        if (ret == Z_STREAM_ERROR) {
            std::cerr << "Error: Zlib compression failed with error code: " << ret << std::endl;
            deflateEnd(&stream);
            return {};
        }

        // Append the compressed data to the output vector
        compressed_data.insert(compressed_data.end(), out_buffer, out_buffer + (sizeof(out_buffer) - stream.avail_out));
    } while (stream.avail_out == 0);  // Continue until the output buffer is filled

    // Clean up the compression stream
    deflateEnd(&stream);

    return compressed_data;  // Return the compressed data
}
