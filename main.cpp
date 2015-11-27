#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "VirtualMachine.h"
#include "BytecodeIO.h"

using namespace std;

int main()
{
    //                                  Create new string, 19 in length to hold the question                                                 Create string & read in result    Print "Hello %name%!"
    std::vector<unsigned char> bytes = {4, 19, 'W', 'h', 'a', 't', ' ', 'I', 's', ' ', 'Y', 'o', 'u', 'r', ' ', 'N', 'a', 'm', 'e', '?', '\n', 0, 4, 0, 6,                     4, 6, 'H', 'e', 'l', 'l', 'o', ' ', 0, 0, 4, 1, '!', 0};
    // std::vector<unsigned char> bytes = {2,0,6,0}; //Get input (character) and print it back out
   // std::vector<unsigned char> bytes = {4, 12, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\n', 0, 5, 0}; //Hello world loop

 //   std::vector<unsigned char> bytes;
  //  BytecodeIO::readBytecode("NameProgram.fry", bytes);
 // BytecodeIO::writeBytecode("NameProgram.fry", bytes);

    VirtualMachine VM;
    VM.interpret(&bytes[0], bytes.size());

    return 0;
}
