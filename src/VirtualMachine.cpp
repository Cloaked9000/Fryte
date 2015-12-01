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

void VirtualMachine::interpret(unsigned char bytecode[], int byteSize)
{
    for(int a = 0; a < byteSize; a++)
    {
        int currentInstruction = bytecode[a];
        switch(currentInstruction)
        {
        case Instruction::CONSOLE_OUT:
        {
            Type variable = pop();
            switch(variable.type)
            {
            case DataType::INT:
                std::cout << variable.intData;
                break;
            case DataType::CHAR:
                std::cout << variable.charData;
                break;
            case DataType::BOOL:
                std::cout << variable.boolData;
                break;
            case DataType::STRING:
                std::cout << *variable.stringData;
                break;
            default:
                throwError(std::string("Failed to CONSOLE_OUT, Unknown data type '" + std::to_string(variable.type) + "'"));
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
        {
            unsigned int stringSize = bytecode[++a]; //String length stored in next byte
            std::string *wholeString = new std::string; //Allocate memory on heap for the string
            wholeString->resize(stringSize);
            for(unsigned int cChar = 0; cChar < stringSize; cChar++) //Read in the string from the bytecode into the allocated memory
                (*wholeString)[cChar] = bytecode[++a];

            push_string(wholeString); //Push the resulting char*
            break;
        }
        case Instruction::GOTO:
            a = static_cast<int>(bytecode[a+1])-1;
            break;
        case Instruction::CONSOLE_IN:
        {
            Type &variable = stack[bytecode[++a]];
            switch(variable.type)
            {
            case DataType::INT:
                std::cin >> variable.intData;
                break;
            case DataType::CHAR:
                std::cin >> variable.charData;
                break;
            case DataType::BOOL:
                std::cin >> variable.boolData;
                break;
            case DataType::STRING:
                std::getline(std::cin, *variable.stringData);
                break;
            default:
                throwError(std::string("Failed to CONSOLE_IN, Unknown data type '" + std::to_string(variable.type) + "'"));
            }
            break;
        }
        case Instruction::MATH_ADD:
            {
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to add from bytecode
                int result = 0;
                for(unsigned int a = 0; a < numberCount; a++) //For the number of arguments specified, pop them all off the stack and add to 'result'
                    result += pop().intData;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_SUBTRACT:
            {
                //TEMPORARY
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                std::vector<int> values;
                int result = 0;
                for(unsigned int a = 0; a < numberCount; a++) //For the number of arguments specified, pop them all off the stack and subtract from 'result'
                    values.emplace_back(pop().intData);
                result = values.back();
                values.pop_back();
                for(auto iter = values.rbegin(); iter != values.rend(); iter++)
                    result -= *iter;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_MULTIPLY:
            {
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to multiply from bytecode
                int result = 0;
                for(unsigned int a = 0; a < numberCount; a++) //For the number of arguments specified, pop them all off the stack and multiply by result
                    result *= pop().intData;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_DIVIDE:
            {
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to divide from bytecode
                int result = 1;
                for(unsigned int a = 0; a < numberCount; a++) //For the number of arguments specified, pop them all off the stack and divide
                    result /= pop().intData;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_MOD:
            {
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to add mod bytecode
                int result = 1;
                for(unsigned int a = 0; a < numberCount; a++) //For the number of arguments specified, pop them all off the stack and add to 'result'
                    result %= pop().intData;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::CLONE_TOP:
            {
                push_type(stack[bytecode[++a]]); //Clone a variable from a position in the stack to the top of the stack
                break;
            }


        default:
            throwError("Unknown instruction '" + std::to_string(currentInstruction) + "'");
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

void VirtualMachine::push_string(std::string* value)
{
    pushStackCheck();

    stack[stackSize++] = Type(value);
}

void VirtualMachine::push_type(Type value)
{
    pushStackCheck();

    stack[stackSize++] = value;
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
        throwError("\nCouldn't pop from stack, stack empty!");
    }
}

void VirtualMachine::pushStackCheck()
{
    if(stackSize == maxStackSize)
    {
        throwError("\nCouldn't push to stack, stack full!");
    }
}

void VirtualMachine::throwError(const std::string& reason)
{
    throw std::string(reason);
}
