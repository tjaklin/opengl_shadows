#pragma once

#include <vector>

struct VertexAttribute
{
    std::vector<float> data;    // Vertex Attribute data.
    unsigned int dimension;     // Dimension of Vertex Attribute data that makes
                                //  up a single Vertex (eg. for a 2D Position Vertex
                                //  Attribute, dimension would be 2).
    unsigned int count;         // Count of elements (floats) in the 'data' object.
};

class VertexAttributeParser
{
public:
    VertexAttributeParser() = default;

    static VertexAttribute ProcessFile(const char* filepath);
};