#pragma once

#include <memory>
#include <string>

namespace huffman {

    // Node of a Hoffman Coding Tree
    struct Node {
        char symbol;
        int freq;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        Node(int init_freq) {freq = init_freq;}
        Node(char init_symbol, int init_freq) {symbol = init_symbol; freq = init_freq;}
    };

    class Encoder {
        private:
            std::shared_ptr<Node> root;
            void buildHuffTree(const std::string& text);
        public:

            // Encodes string given based on Huffman's Algorithm into a Huffman Coding Tree.
            // Stores root of resulting binary tree in private root.
            std::string encode(const std::string& text);
    };

    class Decoder {
        private:
            std::shared_ptr<Node> root;
        public:

            // Decodes string based on Huffman's Algorithm from a Huffman Coding Tree.
            std::string decode(const std::string& text);
    };
}