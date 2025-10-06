#include "huffman.hpp"
#include <unordered_map>
#include <queue>
#include <cstdint>
#include <vector>
#include <stdexcept>

namespace huffman {

    void Decoder::reconHuffTree() {
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Comparator> prio_nodes;
        for (uint16_t i = 0; i < 256; ++i) 
            if (freq_table[i])
                prio_nodes.push(std::make_shared<Node>(i, freq_table[i]));
        
        while(prio_nodes.size() > 1) {
            std::shared_ptr<Node> nd_a = prio_nodes.top();
            prio_nodes.pop();

            std::shared_ptr<Node> nd_b = prio_nodes.top();
            prio_nodes.pop();

            std::shared_ptr<Node> nd_p = std::make_shared<Node>(nd_a->freq + nd_b->freq);

            if (nd_a->freq < nd_b->freq) {
                nd_p->left = nd_a;
                nd_p->right = nd_b;
            } else {
                nd_p->left = nd_b;
                nd_p->right = nd_a;
            }

            if (prio_nodes.empty()) {
                root = nd_p;
                break;
            } else 
                prio_nodes.push(nd_p);
        }

        if (!root) {
            root = prio_nodes.top();
            prio_nodes.pop();
        }
    }

    std::vector<uint8_t> Decoder::decode(const std::vector<uint8_t>& data) {
        std::vector<uint8_t> output;

        std::vector<uint8_t> payload;

        Decoder::deconstructHeader(data, payload);

        Decoder::reconHuffTree();

        return Decoder::decodePayload(payload);
    }

    std::vector<uint8_t> Decoder::deconstructHeader(const std::vector<uint8_t>& data, std::vector<uint8_t>& payload) {
        std::vector<uint8_t> header;

        std::string ver = "";

        uint64_t i;
        
        // Decode version name + number (4 Bytes)
        for (i = 0; i < 4; ++i) 
            ver += data[i];
        
        if (ver != "BRF1")
            std::runtime_error("Bad Magic");

        // Decode flag (MSB = 1) (1 Byte)
        if (data[i++] != 1)
            std::runtime_error("Unsupported Flag");

        // Decode pad_bits (1 Byte)
        pad_bits = data[i++];

        // Decode padding (2 bytes)
        i += 2;

        // Decode org_length -> little-endian (8 bytes)
        org_length = 0;
        for (uint8_t j = 0; j < 8; ++j) 
            org_length = (org_length << 8) | data[i++];
        
        // Decode frequencies (1024 Bytes)
        for (uint16_t j = 0; j < 256; ++j) {
            uint32_t freq = 0;
            freq = data[i++] | data[i++] << 8 | data[i++] << 16 | data[i++] << 24;
            freq_table[j] = freq;
        }

        payload.insert(payload.end(), data.begin() + i, data.end());

        return header;
    }

    std::vector<uint8_t> Decoder::decodePayload(const std::vector<uint8_t>& payload) {
        std::vector<uint8_t> decom_file; // payload to return
        std::shared_ptr<Node> nd = root; // accumulator for where we are in the tree

        for (uint64_t i = 0; i < payload.size() - 1; ++i) {
            uint8_t b = payload[i];
            for (int8_t j = 7; j >= 0; --j) 
                decodeBit(decom_file, (b >> j) & 0x1, nd);
        }

        uint8_t b = payload[payload.size() - 1];
        for (int8_t j = 7; j >= pad_bits; --j)
            decodeBit(decom_file, (b >> j) & 0x1, nd);

        if (nd) 
            nd = root;
        
        return decom_file;
    }

    void Decoder::decodeBit(std::vector<uint8_t>& decom_file, uint8_t bit, std::shared_ptr<Node>& nd) {
        if (!nd) {
            std::runtime_error("Error with Decoding");
            return;
        }
        if (!nd->left && !nd->right) {
            decom_file.push_back(nd->symbol);
            return;
        }
        if (bit)
            nd = nd->right;
        else
            nd = nd->left;
        if (!nd->left && !nd->right) {
            decom_file.push_back(nd->symbol);
            nd = root;
        }
    }
}