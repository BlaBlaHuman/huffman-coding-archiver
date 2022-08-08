#ifndef HW_02_HUFFMAN_DECODER_H
#define HW_02_HUFFMAN_DECODER_H
#include <sstream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "huffman_tree.h"

namespace decoder {
    std::pair<std::size_t, std::size_t> decodeData(std::ifstream& is, std::ofstream& os, std::unordered_map<std::string, char>& table);

	std::vector<bool> getBitsFromChar(const char& ch);

    huffman_tree::HuffmanTreeNode* readTree(std::stringstream& is);
}

#endif
