#include "huffman.hpp"
#include <map>
#include <queue>
#include <cstdint>
#include <vector>

namespace huffman {

    void Encoder::buildHuffTree(const std::vector<uint8_t>& data) {
        std::map<uint8_t, uint32_t> huff_map;

        for (uint8_t b: data)
            ++huff_map[b];

        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Comparator> prio_nodes;

        for (auto p: huff_map) {
            freq_table[p.first] = p.second;
            prio_nodes.push(std::make_shared<Node>(p.first, p.second));
        }
        
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

    std::vector<uint8_t> Encoder::encode(const std::vector<uint8_t>& data) {
        org_length = data.size();

        Encoder::buildHuffTree(data);
        Encoder::dfsHuffTree(root, "");

        std::vector<uint8_t> payload = Encoder::bitPack(data);

        std::vector<uint8_t> header = Encoder::createHeader();

        header.insert(header.end(), payload.begin(), payload.end());

        return header;
    }

    void Encoder::dfsHuffTree(const std::shared_ptr<Node> nd, std::string path) {
        if (!nd) return;
        if (!nd->left && !nd->right) {
            code_table[nd->symbol] = (path.empty()) ? "0": path;
            return;
        }
        dfsHuffTree(nd->left, path + "0");
        dfsHuffTree(nd->right, path + "1");
    }

    std::vector<uint8_t> Encoder::bitPack(const std::vector<uint8_t>& data) {
        std::vector<uint8_t> payload; // payload to return
        uint8_t acc = 0; // accumulator for byte to add to payload
        uint8_t n = 0; // accumulator for where we are in the acc

        for (uint8_t b: data)
            Encoder::appendBits(payload, acc, n, code_table.at(b));

        Encoder::finalizeBits(payload, acc, n);

        pad_bits = (n == 0) ? 0: 8 - n;

        return payload;
    }

    void Encoder::appendBits(std::vector<uint8_t>& payload, uint8_t& acc, uint8_t& n, const std::string& path) {
        for (char c: path) {
            acc = (acc << 1) | ((c == '1') ? 1: 0);
            ++n;
            if (n == 8) {
                payload.push_back(acc);
                n = 0;
                acc = 0;
            }
        }
    }

    void Encoder::finalizeBits(std::vector<uint8_t>& payload, uint8_t& acc, uint8_t& n) {
        if (n == 0) return;
        acc = acc << (8 - n);
        payload.push_back(acc);
    }

    std::vector<uint8_t> Encoder::createHeader() {
        std::vector<uint8_t> header;
        
        // Input version name + number (4 Bytes)
        header.insert(header.end(), {'B', 'R', 'F', '1'});

        // Input flag (MSB = 1) (1 Byte)
        header.insert(header.end(), uint8_t(1));

        // Input pad_bits (1 Byte)
        header.insert(header.end(), pad_bits);

        // Inputs padding (2 bytes)
        header.insert(header.end(), {0, 0});

        // Input org_length -> little-endian (8 bytes)
        for (int i = 0; i < 8; ++i) 
            header.push_back((uint8_t) (org_length >> i * 8));
        
        // Input frequencies (1024 Bytes)
        for (uint32_t f: freq_table) 
            for (uint8_t i = 0; i < 4; ++i) 
                header.push_back((uint8_t) (f >> i * 8));

        return header;
    }
}

