#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

namespace huffman_tree {

    class HuffmanTreeNode {
    public:
        HuffmanTreeNode(char ch, std::size_t freq, HuffmanTreeNode* left, HuffmanTreeNode* right);
        HuffmanTreeNode(char ch, HuffmanTreeNode* left, HuffmanTreeNode* right);
        ~HuffmanTreeNode();
        HuffmanTreeNode* getLeft() const;
        HuffmanTreeNode* getRight() const;
        char getCh() const;
        std::size_t getFreq() const;
        bool isLeaf() const;
    private:
        char ch_;
        std::size_t freq_;
        HuffmanTreeNode* left_;
        HuffmanTreeNode* right_;
    };

    class HuffmanTree {
    public:
        HuffmanTree();
        std::size_t buildTree(std::ifstream& is);
        void generateCodes();
        void setRoot(HuffmanTreeNode* node);
        std::unordered_map<char, std::string> getCodes() const;
		std::unordered_map<std::string, char> getReversedCodes() const;
        HuffmanTreeNode* getRoot();
        ~HuffmanTree();
    private:
        void encode(const huffman_tree::HuffmanTreeNode* root, const std::string& str, std::unordered_map<char, std::string>& huffmanCode);
    private:
        HuffmanTreeNode* root_;
        std::unordered_map<char, std::string> codes_;
    };

    struct tree_comp
    {
        bool operator()(huffman_tree::HuffmanTreeNode* l, huffman_tree::HuffmanTreeNode* r);
    };
}

#endif