#include "huffman_tree.h"
#include <algorithm>
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <fstream>

//huffman_tree::HuffmanTreeNode

huffman_tree::HuffmanTreeNode::HuffmanTreeNode(char ch, std::size_t freq, HuffmanTreeNode* left, HuffmanTreeNode* right): ch_(ch), freq_(freq), left_(left), right_(right) {}
huffman_tree::HuffmanTreeNode::HuffmanTreeNode(char ch, HuffmanTreeNode* left, HuffmanTreeNode* right): ch_(ch), freq_(0), left_(left), right_(right) {}


huffman_tree::HuffmanTreeNode* huffman_tree::HuffmanTreeNode::getLeft() const { return left_; }
huffman_tree::HuffmanTreeNode* huffman_tree::HuffmanTreeNode::getRight() const { return right_; }
char huffman_tree::HuffmanTreeNode::getCh() const { return ch_; }
std::size_t huffman_tree::HuffmanTreeNode::getFreq() const { return freq_; }
bool huffman_tree::HuffmanTreeNode::isLeaf() const { return (left_ == nullptr && right_ == nullptr); }

huffman_tree::HuffmanTreeNode::~HuffmanTreeNode() {
    delete left_;
    delete right_;
}

//huffman_tree::tree_comp

bool huffman_tree::tree_comp::operator()(huffman_tree::HuffmanTreeNode* l, huffman_tree::HuffmanTreeNode* r) { return l->getFreq() >= r->getFreq(); }


//huffmanTree::HuffmanTree

huffman_tree::HuffmanTree::HuffmanTree() : root_(nullptr) {}


void huffman_tree::HuffmanTree::encode(const huffman_tree::HuffmanTreeNode* root, const std::string& str, std::unordered_map<char, std::string> &huffmanCode) {
    if (root == nullptr)
        return;

    if (!root->getLeft() && !root->getRight()) {
        huffmanCode[root->getCh()] = str;
    }

    encode(root->getLeft(), str + "0", huffmanCode);
    encode(root->getRight(), str + "1", huffmanCode);
}

std::size_t huffman_tree::HuffmanTree::buildTree(std::ifstream& is) {
    std::unordered_map<char, std::size_t> freq;
    std::size_t bytes = 0;
    char ch;

    is >> std::noskipws;

    while (is >> ch) {
        bytes++;
        freq[ch]++;
    }

    std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>, tree_comp> pq;

    for (auto pair: freq) {
        huffman_tree::HuffmanTreeNode* node = new huffman_tree::HuffmanTreeNode(pair.first, pair.second, nullptr, nullptr);
        pq.push(node);
    }

    if(pq.size() > 1) {
        while (pq.size() != 1) {
            huffman_tree::HuffmanTreeNode *left = pq.top();
            pq.pop();
            huffman_tree::HuffmanTreeNode *right = pq.top();
            pq.pop();
            int sum = left->getFreq() + right->getFreq();
            huffman_tree::HuffmanTreeNode *node = new huffman_tree::HuffmanTreeNode('\0', sum, left, right);
            pq.push(node);
        }
        root_ = pq.top();
    }
    else if (pq.size() == 1){
        huffman_tree::HuffmanTreeNode *left = pq.top();
        pq.pop();
        huffman_tree::HuffmanTreeNode *right = nullptr;
        huffman_tree::HuffmanTreeNode *node = new huffman_tree::HuffmanTreeNode('\0', left, right);
        pq.push(node);
        root_ = pq.top();
    }


    codes_.clear();
    encode(root_, "", codes_);
    is.clear();
    is.seekg(std::ios::beg);
    return bytes;
}


void huffman_tree::HuffmanTree::generateCodes() {
    codes_.clear();
    encode(root_, "", codes_);
}

void huffman_tree::HuffmanTree::setRoot(huffman_tree::HuffmanTreeNode* node) { root_ = node; }

huffman_tree::HuffmanTreeNode* huffman_tree::HuffmanTree::getRoot() { return root_; }
std::unordered_map<char, std::string> huffman_tree::HuffmanTree::getCodes() const { return codes_; }
std::unordered_map<std::string, char> huffman_tree::HuffmanTree::getReversedCodes() const {
	std::unordered_map<std::string, char> reversed_codes;
	for (auto it = codes_.begin(); it != codes_.end(); ++it) {
		reversed_codes[it->second] = it->first;
	}
	return reversed_codes;
}

huffman_tree::HuffmanTree::~HuffmanTree() { delete root_; }


