#pragma once

#include <vector>

struct ShapeVertices
{
    std::vector<float> data;
    unsigned int size;
};

class VertexParser
{
public:
    VertexParser(const char* filepath);

    bool ProcessFile(); // Try processing the file.

    const std::vector<float>& Data() const;
    unsigned int DataSize() const;

private:
    const char* _filename;
    ShapeVertices _shapeVertices;
    bool _processing_done;  // Processing should only be done once, additional attempts fail.
};