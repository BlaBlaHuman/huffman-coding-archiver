#include "encoder.h"
#include "huffman_tree.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>

std::size_t encoder::writeEncodedMessage(std::ofstream& os, std::ifstream& is, std::unordered_map<char, std::string>& table) {
    std::size_t bytes = 0;
    std::vector<bool> bits;

    is >> std::noskipws;

    char ch;

    while (is >> ch) {
        std::string code = table[ch];
        for (std::size_t j = 0; j < code.size(); ++j) {
            bits.push_back(code[j] - '0');
        }
        if (bits.size() % (sizeof(char) * 8) == 0) {
            for (std::size_t i = 0; i < bits.size() / (sizeof(char) * 8); i += 1) {
                std::vector<bool> chunk(bits.begin() + i * sizeof(char) * 8, bits.begin() + (i + 1) * sizeof(char) * 8);
                char ch = encoder::getCharByBits(chunk);
                os.write(&ch, 1);
                bytes++;
            }
            bits.clear();
        }
    }

    for (std::size_t i = 0; i < bits.size() / (sizeof(char) * 8); i += 1) {
        std::vector<bool> chunk(bits.begin() + i * sizeof(char) * 8, bits.begin() + (i + 1) * sizeof(char) * 8);
        char ch = encoder::getCharByBits(chunk);
        os.write(&ch, 1);
        bytes++;
    }

    char number_of_valid_bits = 8;

    if (bits.size() % 8 != 0) {
        bytes++;


        std::vector<bool> last_chunk = std::vector<bool>(bits.begin() + sizeof(char) * 8 * (bits.size() / (sizeof(char) * 8)), bits.end());

        number_of_valid_bits = bits.end() - (bits.begin() + sizeof(char) * 8 * (bits.size() / (sizeof(char) * 8)));

        while (last_chunk.size() != 8)
            last_chunk.push_back(0);

        char ch = encoder::getCharByBits(last_chunk);
        os.write(&ch, 1);

    }

    os.write(&number_of_valid_bits, 1);

    return bytes;
}


char encoder::getCharByBits(const std::vector<bool>& v) {
    if (v.size() > 8)
        throw std::runtime_error("Input is larger than 8 bits.");

    char ch = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        ch <<= 1;
        ch |= (1 & v[i]);
    }
    return ch;
}

std::size_t encoder::encodeNode(std::ofstream& os, const huffman_tree::HuffmanTreeNode* node) {
    std::size_t bytes = 0;

    if (node != nullptr) {
        if (node->isLeaf()) {
            os.write("1", 1);
            char ch = node->getCh();
            os.write(&ch, 1);
            bytes += 2;
        } else {
            os.write("0", 1);
            bytes++;
            bytes += encodeNode(os, node->getLeft());
            bytes += encodeNode(os, node->getRight());
        }
    }

    return bytes;
}


