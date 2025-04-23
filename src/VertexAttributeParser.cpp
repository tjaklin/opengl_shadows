#include "VertexAttributeParser.hpp"

#include <fstream>
#include <iterator>
#include <vector>

#include <cstdio>

VertexAttribute VertexAttributeParser::ProcessFile(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        printf("[VertexAttributeParser] Error processing file '%s'.\n", filepath);
        return VertexAttribute();
    }

    std::istream_iterator<float> is_iter(file);
    std::istream_iterator<float> eof;

    std::vector<float> buffer(is_iter, eof);

    VertexAttribute va;
    // Dimension is currently fixed to be 3, as this value is the most commonly used
    //  one. Even the values that are 2D (eg. texture coordinates for a 2D texture) are
    //  currently expected to be defined with 3 dimensions (the third compomponent should
    //  be 0.0f in this case).
    va.dimension = 3;
    va.count = buffer.size();
    va.data = std::move(buffer);

    return va;
}