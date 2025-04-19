#include "../inc/VertexAttributeParser.hpp"

#include <fstream>
#include <iterator>
#include <vector>

#include <cstdio>

template<typename T>
VertexAttribute<T> VertexAttributeParser<T>::ProcessFile(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        printf("[VertexAttributeParser] Error processing file '%s'.\n", filepath);
        return VertexAttribute<T>();
    }

    std::istream_iterator<T> is_iter(file);
    std::istream_iterator<T> eof;

    std::vector<T> buffer(is_iter, eof);

    VertexAttribute<T> va;
    // Dimension is currently fixed to be 3, as this value is the most commonly used
    //  one. Even the values that are 2D (eg. texture coordinates for a 2D texture) are
    //  currently expected to be defined with 3 dimensions (the third compomponent should
    //  be 0.0f in this case).
    va.dimension = 3;
    va.count = buffer.size();
    va.data = std::move(buffer);

    return va;
}