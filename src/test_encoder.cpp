#include <cassert>
#include <string>
#include <iostream>
#include "huffman.hpp"

using namespace std;

int main() {
    huffman::Encoder encoder;

    huffman::Decoder decoder;

    const vector<uint8_t> input = {'A', 'A', 'A', 'B', 'B', 'C'};
    const vector<uint8_t> output = encoder.encode(input);
    assert(!output.empty());
    const vector<uint8_t> decode_out = decoder.decode(output);

    const vector<uint8_t> input_a = {'A', 'A', 'A'};
    const vector<uint8_t> output_a = encoder.encode(input);
    assert(!output_a.empty());
    const vector<uint8_t> decode_out_a = decoder.decode(output);

    vector<uint8_t> input_stress;
    for (uint16_t i = 0; i < 256; ++i)
        for (int j = 0; j < 1024; ++j)
            input_stress.push_back((uint8_t) i);
    const vector<uint8_t> output_stress = encoder.encode(input);
    assert(!output_stress.empty());
    const vector<uint8_t> decode_out_stress = decoder.decode(output);

    assert(decoder.decode(output) == input);

    cout << "ALL TESTS PASS" << "\n";

    return 0;
}