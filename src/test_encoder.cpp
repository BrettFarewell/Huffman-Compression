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

    const vector<uint8_t> decode_out = decoder.decode(output);

    assert(decoder.decode(output) == input);

    return 0;
}