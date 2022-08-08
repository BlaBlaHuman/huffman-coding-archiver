#ifndef HW_02_HUFFMAN_ENCODER_H
#define HW_02_HUFFMAN_ENCODER_H
#include <vector>
#include <fstream>
#include "huffman_tree.h"
#include <string>
#include <fstream>

namespace encoder {
    std::size_t writeEncodedMessage(std::ofstream& os, std::ifstream& is, std::unordered_map<char, std::string>& table);

    char getCharByBits(const std::vector<bool>& v);

    std::size_t encodeNode(std::ofstream& os, const huffman_tree::HuffmanTreeNode* node);
}


#endif
