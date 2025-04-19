#pragma once

#include <vector>

// An entity that defines the Vertex Attribute (an object passed into the
//  vertex shader as input).
template<typename T>
struct VertexAttribute
{
    std::vector<T> data;    // Vertex Attribute data.
    unsigned int dimension;     // Dimension of Vertex Attribute data that makes
                                //  up a single Vertex (eg. for a 2D Position Vertex
                                //  Attribute, dimension would be 2).
    unsigned int count;         // Count of elements (floats) in the 'data' object.
                                // NOTE: Currently, every Vertex Attribute is assumed
                                //  to be composed of float values.
};

// An entity that defines the accompanying values
//  for some VertexAttribute object, that are needed
//  when using 'glVertexAttribPointer()' calls.
template<typename T>
struct VertexAttributeLayout
{
  const VertexAttribute<T>& attribute;
  unsigned int location;
  unsigned int stride;
  const unsigned int offset;
};

template <typename T>
class VertexAttributeParser
{
public:
    VertexAttributeParser() = default;

    static VertexAttribute<T> ProcessFile(const char* filepath);
};