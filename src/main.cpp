#include <iostream>
#include <string>
#include "huffman_tree.h"
#include "file_utility.h"
#include "decoder.h"
#include "encoder.h"
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include "constants.h"


void validateArguments(const std::vector<std::string>& arguments, bool& mode, std::string& input_filename, std::string& output_filename) {
    if (arguments.size() != NUMBER_OF_ARGS)
        throw std::runtime_error("Not enough arguments");

    if (std::find(arguments.begin(), arguments.end(), "-c") != arguments.end()) {
        mode = COMPRESSING_MODE;
    }
    else if (std::find(arguments.begin(), arguments.end(), "-u") != arguments.end()) {
        mode = UNCOMPRESSING_MODE;
    }
    else {
        throw std::runtime_error("Mode flag wasn't provided");
    }

    auto f_it = std::find(arguments.begin(), arguments.end(), "-f");
    auto file_it = std::find(arguments.begin(), arguments.end(), "--file");

    if (f_it < arguments.end() - 1) {
        input_filename = *(f_it + 1);
    }
    else if (file_it < arguments.end() - 1) {
        input_filename = *(file_it + 1);
    }
    else {
        throw std::runtime_error("Input file flag wasn't provided correctly");
    }

    auto o_it = std::find(arguments.begin(), arguments.end(), "-o");
    auto output_it = std::find(arguments.begin(), arguments.end(), "--output");

    if (o_it < arguments.end() - 1) {
        output_filename = *(o_it + 1);
    }
    else if (output_it < arguments.end() - 1) {
        output_filename = *(output_it + 1);
    }
    else {
        throw std::runtime_error("Output file flag wasn't provided correctly");
    }

	if (!std::ifstream(input_filename).good() || !std::ofstream(output_filename).good())
		throw std::runtime_error("Can not open given files");
}

void compressingMode(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream input_stream(input_filename);
    std::ofstream output_stream(output_filename, std::ios::out | std::ios::binary);
    huffman_tree::HuffmanTree tree;

    std::size_t input_bytes = tree.buildTree(input_stream);

    std::size_t table_bytes = file_utility::writeTable(output_stream, tree);

	std::unordered_map<char, std::string> codes = tree.getCodes();

    std::size_t message_bytes = encoder::writeEncodedMessage(output_stream, input_stream, codes);

    std::cout << input_bytes << std::endl << message_bytes << std::endl << table_bytes + NUMBER_OF_SERVICE_BYTES_FOR_LAST_UNFILLED_BYTE << std::endl;
}

void uncompressingMode(const std::string& input_filename, const std::string& output_filename) {
    std::ifstream input_stream(input_filename, std::ios::in | std::ios::binary);
    std::ofstream output_stream(output_filename);
    huffman_tree::HuffmanTree tree;

    std::size_t table_bytes = file_utility::readTable(input_stream, tree);

    tree.generateCodes();

	std::unordered_map<std::string, char> codes = tree.getReversedCodes();

    std::pair<std::size_t, std::size_t> input_output_bytes = decoder::decodeData(input_stream, output_stream, codes);

    std::cout << input_output_bytes.first - NUMBER_OF_SERVICE_BYTES_FOR_LAST_UNFILLED_BYTE << std::endl << input_output_bytes.second << std::endl << table_bytes + NUMBER_OF_SERVICE_BYTES_FOR_LAST_UNFILLED_BYTE << std::endl;
}

void run(const std::vector<std::string>& arguments) {
    bool mode;
    std::string input_filename, output_filename;

    validateArguments(arguments, mode, input_filename, output_filename);

    if (mode == COMPRESSING_MODE) {
        compressingMode(input_filename, output_filename);
    }
    else if (mode == UNCOMPRESSING_MODE) {
        uncompressingMode(input_filename, output_filename);
    }

}

int main(int argc, char* argv[]) {
    try {
        std::vector<std::string> arguments(argv + 1, argv + argc);
        run(arguments);
    }
    catch (const std::exception& e) {
        return 1;
    }
    return 0;
}