#include <cassert>
#include <string>
#include "huffman.hpp"

using namespace std;

int main() {
    huffman::Encoder encoder;

    huffman::Decoder decoder;

    const vector<uint8_t> input = {'A', 'A', 'A', 'B', 'B', 'C'};

    const vector<uint8_t> output = encoder.encode(input);

    assert(!output.empty());

    assert(decoder.decode(output) == input);

    assert(encoder.encode(vector<uint8_t>()) == vector<uint8_t>());
    assert(decoder.decode(vector<uint8_t>()) == vector<uint8_t>());

    return 0;
}