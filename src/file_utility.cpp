#include "file_utility.h"
#include "huffman_tree.h"
#include "decoder.h"
#include "encoder.h"
#include "constants.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <queue>


std::size_t file_utility::readTable(std::ifstream& is, huffman_tree::HuffmanTree& tree) {
    std::vector<char> delim = {END_OF_TABLE_CHAR, DELIMITER_CHAR, DELIMITER_CHAR};

	is >> std::noskipws;

	std::vector<char> bytes;

	char ch;
	auto it = std::search(bytes.begin(), bytes.end(), delim.begin(), delim.end());

	while (it == bytes.end()) {
		is >> ch;
		bytes.push_back(ch);
		it = std::search(bytes.begin(), bytes.end(), delim.begin(), delim.end());
	}

    std::stringstream ss(std::string(bytes.begin(), it));
    ss >> std::noskipws;


    huffman_tree::HuffmanTreeNode* root = decoder::readTree(ss);
    tree.setRoot(root);


    return bytes.size();
}

std::size_t file_utility::writeTable(std::ofstream& os, huffman_tree::HuffmanTree& tree) {
    std::size_t bytes = encoder::encodeNode(os, tree.getRoot());

    os.write(&END_OF_TABLE_CHAR, 1);
    os.write(&DELIMITER_CHAR, 1);
    os.write(&DELIMITER_CHAR, 1);
    bytes += 3;

    return bytes;
}











