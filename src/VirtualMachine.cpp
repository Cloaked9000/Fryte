#include "VirtualMachine.h"

VirtualMachine::VirtualMachine()
{
    //ctor
    stackSize = 0;
}

VirtualMachine::~VirtualMachine()
{
    //dtor
}

void VirtualMachine::interpret(char bytecode[], unsigned int byteSize)
{
    for(unsigned int a = 0; a < byteSize; a++)
    {
        const char &currentInstruction = bytecode[a];
        switch(currentInstruction)
        {
        case Instruction::CONSOLE_OUT:
            {
                Type popped = pop();
                switch(popped.type)
                {
                    case DataType::INT:
                        std::cout << popped.intData;
                        break;
                    case DataType::CHAR:
                        std::cout << popped.charData;
                        break;
                    case DataType::BOOL:
                        std::cout << popped.boolData;
                        break;
                    case DataType::STRING:
                        std::cout << popped.stringData;
                        break;
                }
            }
            break;
        case Instruction::CREATE_INT:
            push_integer(bytecode[++a]);
            break;
        case Instruction::CREATE_CHAR:
            push_char(bytecode[++a]);
            break;
        case Instruction::CREATE_BOOL:
            push_bool(bytecode[++a]);
            break;
        case Instruction::CREATE_STRING:
            unsigned int stringSize = bytecode[++a]; //String length stored in next byte
            char *wholeString = new char[stringSize]; //Allocate memory on heap for the string
            for(unsigned int cChar = 0; cChar < stringSize; cChar++) //Read in the string from the bytecode into the allocated memory
                wholeString[cChar] = bytecode[++a];
            push_string(wholeString); //Push the resulting char*
            break;
        }
    }
}

void VirtualMachine::push_integer(int value)
{
    pushStackCheck();

    stack[stackSize++] = Type(value);
}

void VirtualMachine::push_char(unsigned char value)
{
    pushStackCheck();

    stack[stackSize++] = Type(value);
}

void VirtualMachine::push_bool(bool value)
{
    pushStackCheck();

    stack[stackSize++] = Type(value);
}

void VirtualMachine::push_string(char* value)
{
    pushStackCheck();

    stack[stackSize++] = Type(value);
}

Type VirtualMachine::pop()
{
    popStackCheck();

    return stack[--stackSize];
}

void VirtualMachine::popStackCheck()
{
    if(stackSize == 0)
    {
        std::cout << "\nCouldn't pop from stack, stack empty!";
        throw 1;
    }
}

void VirtualMachine::pushStackCheck()
{
    if(stackSize == maxStackSize)
    {
        std::cout << "\nCouldn't push to stack, stack full!";
        throw 2;
    }
}
