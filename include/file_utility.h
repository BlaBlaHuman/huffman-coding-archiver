#ifndef HW_02_HUFFMAN_FILEMANAGER_H
#define HW_02_HUFFMAN_FILEMANAGER_H
#include <fstream>
#include "huffman_tree.h"

namespace file_utility {
    std::size_t writeTable(std::ofstream& os, huffman_tree::HuffmanTree& tree);
	std::size_t readTable(std::ifstream& is, huffman_tree::HuffmanTree& tree);
}


#endif