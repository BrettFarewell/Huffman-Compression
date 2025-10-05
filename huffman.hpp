#pragma once

#include <memory>
#include <cstdint>
#include <vector>

namespace huffman {

    // Node of a Hoffman Coding Tree
    struct Node {
        uint8_t symbol;
        int freq;
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        Node(int init_freq) {freq = init_freq;}
        Node(char init_symbol, int init_freq) {symbol = init_symbol; freq = init_freq;}
    };

    class Encoder {
        private:
            std::shared_ptr<Node> root;
            void buildHuffTree(const std::vector<uint8_t>& text);
            std::vector<uint8_t> write(const std::vector<uint8_t>& text);
        public:

            // Encodes string given based on Huffman's Algorithm into a Huffman Coding Tree.
            // Stores root of resulting binary tree in private root.
            std::vector<uint8_t> encode(const std::vector<uint8_t>& text);
    };

    class Decoder {
        private:
            std::shared_ptr<Node> root;
        public:

            // Decodes string based on Huffman's Algorithm from a Huffman Coding Tree.
            std::vector<uint8_t> decode(const std::vector<uint8_t>& text);
    };
}