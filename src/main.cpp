#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "VirtualMachine.h"

using namespace std;

int main()
{
   // std::vector<char> bytes = {1,10,0,1,120,0,1,40,0};
   //std::vector<char> bytes = {2, 'H', 0, 2, 'e', 0, 2, 'l', 0, 2, 'l', 0, 2, 'o', 0, 2, ' ', 0, 2, 'W', 0, 2, 'o', 0, 2, 'r', 0, 2, 'l', 0, 2, 'd', 0, 2, '!', 0, };

    std::vector<char> bytes = {4, 11, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0};

    VirtualMachine VM;
    VM.interpret(&bytes[0], bytes.size());

    return 0;
}
