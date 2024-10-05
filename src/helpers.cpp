#include "headers/helpers.h"
#include <ostream>
#include <fstream>
#include <vector>
#include <zlib.h>

/*
 * Returns the compressed data from the specified git object file as a vector of unsigned chars.
 *
 * */
std::vector<unsigned char> read_git_object(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    std::vector<unsigned char> data;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file at " << filepath << std::endl;
        return data;
    }

    data.assign(
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );

    return data;
}

/*
 * Returns the decompressed data from the compressed data previously loaded and stored in a vector of unsigned chars.
 *
 * */
std::vector<unsigned char> decompress_git_object(const std::vector<unsigned char>& compressed_data) {
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = compressed_data.size();
    stream.next_in = const_cast<Bytef*>(compressed_data.data());

    if (inflateInit(&stream) != Z_OK) {
        std::cerr << "Error: Failed to initialize zlib decompression" << std::endl;
        return {};
    }

    std::vector<unsigned char> decompressed_data;
    unsigned char out_buffer[8192];

    int ret;
    do {
        stream.avail_out = sizeof(out_buffer);
        stream.next_out = out_buffer;

        ret = inflate(&stream, Z_NO_FLUSH);

        if (ret == Z_MEM_ERROR || ret == Z_DATA_ERROR || ret == Z_STREAM_ERROR) {
            std::cerr << "Error: Zlib decompression failed with error code: " << ret << std::endl;
            inflateEnd(&stream);
            return {};
        }

        decompressed_data.insert(decompressed_data.end(), out_buffer, out_buffer + (sizeof(out_buffer) - stream.avail_out));
    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    return decompressed_data;
}

std::tuple<std::string, size_t, size_t> get_git_object_type_and_size(const std::vector<unsigned char>& decompressed_data) {
    size_t index = 0;
    std::string type;
    std::string size_str;

    // Read object type
    while (index < decompressed_data.size() && decompressed_data[index] != ' ') {
        type += static_cast<char>(decompressed_data[index]);
        index++;
    }
    index++; // Skip space

    // Read object size
    while (index < decompressed_data.size() && decompressed_data[index] != '\0') {
        size_str += static_cast<char>(decompressed_data[index]);
        index++;
    }
    index++; // Skip null character

    size_t size = std::stoi(size_str); // Convert size string to size_t

    return {type, size, index}; // Return type, size, and index
}

void print_git_object(const std::vector<unsigned char>& decompressed_data) {

    auto [type, size, index] = get_git_object_type_and_size(decompressed_data);

    if (type == "blob") {
        std::cout << "Content:\n";
        for (size_t i = 0; i < size; i++) {
            std::cout << static_cast<char>(decompressed_data[index++]);
        }
    } else if (type == "commit") {
        std::cout << "Commit Content:\n";
        while (index < decompressed_data.size()) {
            std::cout << static_cast<char>(decompressed_data[index++]);
        }
    } else if (type == "tree") {
        std::cout << "Tree Entries:\n";
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

            std::cout << mode << " " << filename << "\n";
        }
    } else {
        std::cerr << "Unknown object type: " << type << std::endl;
    }
}
