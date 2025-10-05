#pragma once

#include <memory>
#include <unordered_map>
#include <cstdint>
#include <queue>
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
            uint32_t freq_table[256] = {0};
            std::shared_ptr<Node> root;
            std::unordered_map<uint8_t, std::string> code_table;
            uint8_t pad_bits;
            uint64_t org_length;

            void buildHuffTree(const std::vector<uint8_t>& text);
            void dfsHuffTree(const std::shared_ptr<Node> nd, std::string path);
            std::vector<uint8_t> bitPack(const std::vector<uint8_t>& data);
            void appendBits(std::vector<uint8_t>& payload, uint8_t& acc, uint8_t& n, const std::string& path);
            void finalizeBits(std::vector<uint8_t>& payload, uint8_t& acc, uint8_t& n);
            std::vector<uint8_t> createHeader();
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