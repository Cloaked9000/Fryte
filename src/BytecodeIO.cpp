#include "BytecodeIO.h"

BytecodeIO::BytecodeIO()
{
    //ctor
}

BytecodeIO::~BytecodeIO()
{
    //dtor
}

bool BytecodeIO::readBytecode(const std::string& filepath, std::vector<unsigned int>& byteCode)
{
    std::streampos fileSize;
    unsigned char * memblock;

    std::ifstream file;
    file.open(filepath, std::ios::in | std::ios::binary | std::ios::ate);
    if(!file.is_open())
        return false;

    fileSize = file.tellg();
    memblock = new unsigned char [fileSize];
    file.seekg(0, std::ios::beg);
    file.read((char*)memblock, fileSize);
    file.close();

    for(unsigned int a = 0; a < fileSize; a++)
        byteCode.emplace_back(static_cast<unsigned int>(memblock[a]));

    delete[] memblock;
    return true;
}

bool BytecodeIO::writeBytecode(const std::string& filepath, const std::vector<unsigned int>& byteCode)
{
    std::ofstream file;
    file.open(filepath, std::ios::out | std::ios::binary);
    if(!file.is_open())
        return false;

    for(const auto &byte: byteCode)
        file << byte;

    file.close();
    return true;
}
