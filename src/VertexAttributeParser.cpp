#include "../inc/VertexAttributeParser.hpp"

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
    va.dimension = 3;
    va.count = buffer.size();
    va.data = std::move(buffer);

    return va;
}