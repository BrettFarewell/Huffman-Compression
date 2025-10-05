#include <cassert>
#include <string>
#include "huffman_encode.hpp"

using namespace std;

int main() {
    huffman::Encoder encoder;

    huffman::Decoder decoder;

    const string input = "AAABBC";

    const string output = encoder.encode(input);

    assert(!output.empty());

    assert(decoder.decode(output) == input);

    assert(encoder.encode("") == "");
    assert(decoder.decode("") == "");

    return 0;
}