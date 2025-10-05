#include "huffman.hpp"
#include <unordered_map>
#include <queue>
#include <vector>

namespace huffman {

    struct Comparator {
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return a->freq < b->freq;
        }
    };
    
    void Encoder::buildHuffTree(const std::string& text) {
        std::unordered_map<char, int> huff_map;

        for (char c: text)
            ++huff_map[c];

        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Comparator> prio_nodes;

        for (auto p: huff_map) 
            prio_nodes.push(std::make_shared<Node>(p.first, p.second));
        
        while(!prio_nodes.empty()) {
            std::shared_ptr<Node> nd_a = prio_nodes.top();
            prio_nodes.pop();

            std::shared_ptr<Node> nd_b = prio_nodes.top();
            prio_nodes.pop();

            std::shared_ptr<Node> nd_p = std::make_shared<Node>(nd_a->freq + nd_b->freq);

            if (nd_a->freq <= nd_b->freq) {
                nd_p->left = nd_a;
                nd_p->right = nd_b;
            } else {
                nd_p->left = nd_b;
                nd_p->right = nd_a;
            }

            if (!prio_nodes.empty()) {
                Encoder::root = nd_p;
                break;
            } else 
                prio_nodes.push(nd_p);
        }
    }

    std::string Encoder::encode(const std::string& text) {
        Encoder::buildHuffTree(text);
        return ""; // stub
    }
}

