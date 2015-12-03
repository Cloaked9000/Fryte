#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "VirtualMachine.h"
#include "BytecodeIO.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc < 2)
        return -1;

    std::vector<unsigned char> bytes;
    BytecodeIO::readBytecode(argv[1], bytes);

    VirtualMachine VM;

    try
    {
        VM.interpret(&bytes[0], bytes.size());
    }
    catch(const std::string &e)
    {
        std::cout << "\nException: " << e;
    }
    catch(const std::exception &e)
    {
        std::cout << "\nException: " << e.what();
    }
    return 0;
}
