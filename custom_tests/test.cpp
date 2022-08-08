#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "encoder.h"
#include "huffman_tree.h"
#include "decoder.h"
#include "file_utility.h"

#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <iostream>

namespace huffmanTree_tests {

	TEST_SUITE("HuffmanTreeNode tests") {
		huffman_tree::HuffmanTreeNode* node1 = new huffman_tree::HuffmanTreeNode('a', 45, nullptr, nullptr);
        huffman_tree::HuffmanTreeNode* node2 = new huffman_tree::HuffmanTreeNode('b', node1, nullptr);

		TEST_CASE("test getLeft()") {
            CHECK_EQ(node1->getLeft(), nullptr);
			CHECK_EQ(node2->getLeft(), node1);
        }

		TEST_CASE("test getRight()") {
            CHECK_EQ(node1->getRight(), nullptr);
			CHECK_EQ(node2->getRight(), nullptr);
        }

		TEST_CASE("test getCh()") {
            CHECK_EQ(node1->getCh(), 'a');
			CHECK_EQ(node2->getCh(), 'b');
        }

		TEST_CASE("test getFreq()") {
            CHECK_EQ(node1->getFreq(), 45);
			CHECK_EQ(node2->getFreq(), 0);
        }

		TEST_CASE("test isLeaf()") {
            CHECK_EQ(node1->isLeaf(), true);
			CHECK_EQ(node2->isLeaf(), false);
        }

    }

	TEST_SUITE("HuffmanTree tests") {
		huffman_tree::HuffmanTree tree;

		TEST_CASE("test getRoot") {
			CHECK_EQ(tree.getRoot(), nullptr);
        }

		TEST_CASE("test buildTree") {
			std::ifstream is("custom_tests/tests_input_samples/test_file_1.txt");
			tree.buildTree(is);
			std::unordered_map<char, std::string> codes = tree.getCodes();
			CHECK_LT(codes['a'].size(), codes['n'].size());
			CHECK_LT(codes['a'].size(), codes['h'].size());

			tree.generateCodes();
			std::unordered_map<char, std::string> new_codes = tree.getCodes();
			CHECK_EQ(new_codes, codes);
        }


		TEST_CASE("test setRoot") {
			huffman_tree::HuffmanTreeNode* node = new huffman_tree::HuffmanTreeNode('b', nullptr, nullptr);
			tree.setRoot(node);
			CHECK_EQ(node, tree.getRoot());
        }



    }

}

namespace encoder_tests {
	TEST_SUITE("getCharByBits") {
		TEST_CASE("single bit 1") {
			std::vector<bool> v = {1};
			CHECK_EQ(encoder::getCharByBits(v), char(1));
		}
		TEST_CASE("single bit 0") {
			std::vector<bool> v = {0};
			CHECK_EQ(encoder::getCharByBits(v), char(0));
		}
		TEST_CASE("eight bits") {
			std::vector<bool> v = {1, 1, 1, 0, 1, 0, 1, 0};
			CHECK_EQ(encoder::getCharByBits(v), char(234));
		}
		TEST_CASE("eight zero bits") {
			std::vector<bool> v = {0, 0, 0, 0, 0, 0, 0, 0};
			CHECK_EQ(encoder::getCharByBits(v), char(0));
		}
		TEST_CASE("more than 8 bits") {
			std::vector<bool> v = {0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
			CHECK_THROWS_AS(encoder::getCharByBits(v), std::runtime_error);
		}
	}

	TEST_SUITE("encodeNode") {
        TEST_CASE("Random") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/test_file_1.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_encodeNode.bin", std::ios::out | std::ios::binary);
			tree.buildTree(is);
			std::size_t bytes = file_utility::writeTable(os, tree);
			is.close();
			os.close();
			is = std::ifstream("custom_tests/tests_output_samples/test_encodeNode.bin", std::ios::in | std::ios::binary);
			std::stringstream buffer;
			buffer << is.rdbuf();
			std::string s = buffer.str();
			CHECK_EQ(s, "0001m1u1b01a01l01n1h-\n\n");
			CHECK_EQ(bytes, 23);
        }
		TEST_CASE("Empty") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/empty.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_encodeNode.txt");
			tree.buildTree(is);
			std::size_t bytes = file_utility::writeTable(os, tree);
			is.close();
			os.close();
			is = std::ifstream("custom_tests/tests_output_samples/test_encodeNode.txt");
			std::stringstream buffer;
			buffer << is.rdbuf();
			std::string s = buffer.str();
			CHECK_EQ(s, "-\n\n");
			CHECK_EQ(bytes, 3);
        }
    }

}


namespace decoder_tests {

	TEST_SUITE("getBitsFromChar") {
        TEST_CASE("zero char") {
            CHECK_EQ(decoder::getBitsFromChar(char(0)),  std::vector<bool>{0, 0, 0, 0, 0, 0, 0, 0});
        }
		TEST_CASE("full 1 char") {
            CHECK_EQ(decoder::getBitsFromChar(char(255)), std::vector<bool>{1, 1, 1, 1, 1, 1, 1, 1});
        }
		TEST_CASE("random chars") {
            CHECK_EQ(decoder::getBitsFromChar(char(43)), std::vector<bool>{0, 0, 1, 0, 1, 0, 1, 1});
			CHECK_EQ(decoder::getBitsFromChar(char(145)), std::vector<bool>{1, 0, 0, 1, 0, 0, 0, 1});
        }

    }

	TEST_SUITE("readTree") {
        TEST_CASE("Random") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/test_file_2.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_readTree.bin", std::ios::out | std::ios::binary);
			tree.buildTree(is);
			std::unordered_map<char, std::string> codes = tree.getCodes();
			file_utility::writeTable(os, tree);
			is.close();
			os.close();
			is = std::ifstream("custom_tests/tests_output_samples/test_readTree.bin", std::ios::in | std::ios::binary);
			std::stringstream buffer;
			buffer << is.rdbuf();
			huffman_tree::HuffmanTreeNode* node = decoder::readTree(buffer);
			tree.setRoot(node);
			tree.generateCodes();
			std::unordered_map<char, std::string> new_codes = tree.getCodes();
			CHECK_EQ(codes, new_codes);
        }


    }

}

namespace file_utility_tests {
	TEST_SUITE("readTable") {
		TEST_CASE("Empty") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/empty.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_readTable.bin", std::ios::out | std::ios::binary);
			tree.buildTree(is);
			std::unordered_map<char, std::string> codes = tree.getCodes();
			file_utility::writeTable(os, tree);
			is.close();
			os.close();
			is = std::ifstream("custom_tests/tests_output_samples/test_readTable.bin", std::ios::in | std::ios::binary);
			std::size_t bytes = file_utility::readTable(is, tree);
			CHECK_EQ(bytes, 3);
		}
		TEST_CASE("Random") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/test_file_1.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_readTable.bin", std::ios::out | std::ios::binary);
			tree.buildTree(is);
			std::unordered_map<char, std::string> codes = tree.getCodes();
			file_utility::writeTable(os, tree);
			is.close();
			os.close();
			is = std::ifstream("custom_tests/tests_output_samples/test_readTable.bin", std::ios::in | std::ios::binary);
			std::size_t bytes = file_utility::readTable(is, tree);
			CHECK_EQ(bytes, 23);
		}
	}

	TEST_SUITE("writeTable") {
		TEST_CASE("Empty") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/empty.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_writeTable.bin", std::ios::out | std::ios::binary);
			tree.buildTree(is);
			std::unordered_map<char, std::string> codes = tree.getCodes();
			std::size_t bytes = file_utility::writeTable(os, tree);
			CHECK_EQ(bytes, 3);
		}
		TEST_CASE("Random") {
			huffman_tree::HuffmanTree tree;
			std::ifstream is("custom_tests/tests_input_samples/test_file_1.txt");
			std::ofstream os("custom_tests/tests_output_samples/test_writeTable.bin", std::ios::out | std::ios::binary);
			tree.buildTree(is);
			std::unordered_map<char, std::string> codes = tree.getCodes();
			std::size_t bytes = file_utility::writeTable(os, tree);
			CHECK_EQ(bytes, 23);
		}
	}

}
