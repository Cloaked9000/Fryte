#ifndef BYTECODEIO_H
#define BYTECODEIO_H

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class BytecodeIO
{
public:
    BytecodeIO();
    virtual ~BytecodeIO();

    static bool writeBytecode(const std::string &filepath, const std::vector<unsigned int> &byteCode);
    static bool readBytecode(const std::string &filepath, std::vector<unsigned int> &byteCode);

protected:
private:
};

#endif // BYTECODEIO_H
