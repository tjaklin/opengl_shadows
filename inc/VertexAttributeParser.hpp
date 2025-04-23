#pragma once

#include <vector>

// An entity that defines the Vertex Attribute (an object passed into the
//  vertex shader as input).
struct VertexAttribute
{
    std::vector<float> data;    // Vertex Attribute data.
    unsigned int dimension;     // Dimension of Vertex Attribute data that makes
                                //  up a single Vertex (eg. for a 2D Position Vertex
                                //  Attribute, dimension would be 2).
    unsigned int count;         // Count of elements (floats) in the 'data' object.
                                // NOTE: Currently, every Vertex Attribute is assumed
                                //  to be composed of 'float' values.
};

// An entity that defines the accompanying values
//  for some VertexAttribute object, that are needed
//  when using 'glVertexAttribPointer()' calls.
struct VertexAttributeLayout
{
  const VertexAttribute& attribute;
  unsigned int location;
  unsigned int stride;
  const unsigned int offset;
};

class VertexAttributeParser
{
public:
    VertexAttributeParser() = default;

    static VertexAttribute ProcessFile(const char* filepath);
};