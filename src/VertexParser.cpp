#include "../inc/VertexParser.hpp"

#include <fstream>
#include <sstream>

#include <string>
#include <vector>

#include <iterator>

#include <cstdio>

VertexParser::VertexParser(const char* filename)
    : _filename(filename), _shapeVertices(), _processing_done(false)
{}

bool VertexParser::ProcessFile()
{
    if (_processing_done) { return true; }

    // Try opening the file.
    std::ifstream input(_filename);
    if (!input.is_open())
    {
        printf("[VertexParser] Cannot process file.\n");
        return false;
    }

// OVO JE SAMO TEST - POCETAK
if (false)
{
    // TODO:
    // Nebi htel da mi 'istream_iterator' napuni vector<string>,
    // vec bi htel da mi sav sadrzaj 'istream_iteratora' bude
    // zapisan u jedan-jedini 'string' objekt jer moram to kasnije
    // proslijedit ka OpenGL masini (koja ocekuje da bude upravo c-string).
    // Mozda mogu za te potrebe koristiti 'stringstream'.
    // Ili, mogu prvo spremit u vector<string> i onda iz njega, pomocu
    // algoritma 'copy', kopirat u 'stringstream' pa iznjega u string?
    std::istream_iterator<std::string> is_iter(input);
    std::istream_iterator<std::string> eof;

    std::vector<std::string> buffer(is_iter, eof);
    printf("Start Buffer.\n");
    for (auto s : buffer)
    {
        printf("%s\n", s.c_str());
    }
    printf("\nEnd Buffer.\n");
}
// OVO JE SAMO TEST - KRAJ

    // Step 1.1: This is a temporary solution. Stop all further processing and
    //  return this:
    _shapeVertices.data =
    {
        // Position vertex attribute
        -1.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f, 
         1.0f, 1.0f, 0.0f, 
         1.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
         1.0f,-1.0f, 0.0f
    };
    /*
    _shapeVertices.data =
    {   // First triangle
        -0.4f, -0.4f, 0.0f,
        -0.4f,  0.4f, 0.0f,
         0.4f,  0.4f, 0.0f,

        // Second triangle
         0.4f,  0.4f, 0.0f,
         0.4f, -0.4f, 0.0f,
        -0.4f, -0.4f, 0.0f
    };
    */
    _shapeVertices.size = _shapeVertices.data.size();
    _processing_done = true;
    return true;

    // Step 2: Process the file line by line.

    // TODO: TU SAM STAL!! Htel sam pogledati kaj Bjarne pise
    //  u knjigi da je po njemu najbolji nacin za raditi citanje
    //  iz fajla i procesuiranje podataka iz fajla (npr podjela 
    //  retka na tokene i takve stvari)

    // Step 2.1: Each line consists of multiple float values
    //  may/may not contain a prefix '-' / '+' and two separate
    //  values are separated by a ',' (comma) char.
    //
    //  Split each line into tokens (separated by ',' delimiter).

    // Step 2.2: Treat each token as a float number and save it
    //  to a local buffer.

    // Step 3: Close the file.

    // Step 4: If everything went fine, return 'true'. Otherwise 'false'.

    return true;
}

const std::vector<float>& VertexParser::Data() const
{
    return _shapeVertices.data;
}

unsigned int VertexParser::DataSize() const
{
    return _shapeVertices.size;
}