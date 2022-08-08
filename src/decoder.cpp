#include "decoder.h"
#include "encoder.h"
#include "huffman_tree.h"
#include "constants.h"
#include <sstream>
#include <bitset>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <stdexcept>

std::pair<std::size_t, std::size_t> decoder::decodeData(std::ifstream& is, std::ofstream& os, std::unordered_map<std::string, char>& table) {
    std::vector<bool> bits;
	std::size_t input_bytes = 0;
	std::size_t output_bytes = 0;
	os << std::noskipws;
	char ch, service_byte;
	if (is.read(&ch, 1)) {
		input_bytes++;
	}

	if (is.read(&service_byte, 1)) {
		input_bytes++;
	}
	std::size_t i = 0;
	std::string buffer = "";

	char tmp;
	while (is.read(&tmp, 1)) {
		std::vector<bool> bits_from_char = getBitsFromChar(ch);
		bits.insert(bits.end(), bits_from_char.begin(), bits_from_char.end());

		for (; i < bits.size(); ++i) {
			buffer += '0' + bits[i];
			if (table.find(buffer) != table.end()) {
				output_bytes++;
				os << table[buffer];
				buffer = "";
			}
		}
		ch = service_byte;
		service_byte = tmp;
		input_bytes++;
	}


	if (input_bytes >= 2) {
		std::vector<bool> bits_from_char = getBitsFromChar(ch);
		bits_from_char.erase(bits_from_char.begin() + service_byte, bits_from_char.end());
		bits.insert(bits.end(), bits_from_char.begin(), bits_from_char.end());

		for (; i < bits.size(); ++i) {
			buffer += '0' + bits[i];
			if (table.find(buffer) != table.end()) {
				output_bytes++;
				os << table[buffer];
				buffer = "";
			}
		}
	}

	return std::make_pair(input_bytes, output_bytes);
}


std::vector<bool> decoder::getBitsFromChar(const char& ch) {
	std::string s = std::bitset<8>(ch).to_string();;
	std::vector<bool> v;
	for (std::size_t i = 0; i < s.size(); ++i) {
		v.push_back(s[i] - '0');
	}
	return v;
}

huffman_tree::HuffmanTreeNode* decoder::readTree(std::stringstream& is) {
    char ch;
	is >> std::noskipws;
    if (!(is >> ch)) {
        return nullptr;
    }

    if (ch == '1') {
        is >> ch;
        return new huffman_tree::HuffmanTreeNode(ch, nullptr, nullptr);
    } else {
        huffman_tree::HuffmanTreeNode *left = readTree(is);
        huffman_tree::HuffmanTreeNode *right = readTree(is);
        return new huffman_tree::HuffmanTreeNode('\0', left, right);
    }
}