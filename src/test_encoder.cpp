#include <cassert>
#include <string>
#include <iostream>
#include "huffman.hpp"

using namespace std;

int main() {
    huffman::Encoder encoder1;
    huffman::Encoder encoder2;
    huffman::Encoder encoder3;
    huffman::Encoder encoder4;

    huffman::Decoder decoder1;
    huffman::Decoder decoder2;
    huffman::Decoder decoder3;
    huffman::Decoder decoder4;

    const vector<uint8_t> input = {'A', 'A', 'A', 'B', 'B', 'C'};
    const vector<uint8_t> output = encoder1.encode(input);
    assert(!output.empty());
    const vector<uint8_t> decode_out = decoder1.decode(output);

    const vector<uint8_t> input_a = {'A', 'A', 'A'};
    const vector<uint8_t> output_a = encoder2.encode(input_a);
    assert(!output_a.empty());
    const vector<uint8_t> decode_out_a = decoder2.decode(output_a);

    vector<uint8_t> input_stress;
    for (uint16_t i = 0; i < 256; ++i)
        for (int j = 0; j < 1024; ++j)
            input_stress.push_back((uint8_t) i);
    const vector<uint8_t> output_stress = encoder3.encode(input_stress);
    assert(!output_stress.empty());
    const vector<uint8_t> decode_out_stress = decoder3.decode(output_stress);

    vector<uint8_t> input_uneven_stress;
    for (uint16_t i = 0; i < 256; ++i)
        for (int j = 0; j < 8096 / (i + 1); ++j)
            input_uneven_stress.push_back((uint8_t) i);
    const vector<uint8_t> output_uneven_stress = encoder3.encode(input_uneven_stress);
    assert(!output_uneven_stress.empty());
    const vector<uint8_t> decode_out_uneven_stress = decoder3.decode(output_uneven_stress);

    assert(decode_out == input);
    assert(decode_out_a == input_a);
    assert(decode_out_stress == input_stress);
    assert(decode_out_uneven_stress == input_uneven_stress);

    cout << "ALL TESTS PASS" << "\n";

    return 0;
}