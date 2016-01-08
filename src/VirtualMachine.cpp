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

void VirtualMachine::interpret(unsigned int bytecode[], int byteSize)
{
    auto startPoint = std::chrono::system_clock::now();

    for(int a = 0; a < byteSize; ++a)
    {
        switch(bytecode[a])
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
            break;
        }
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
            std::string wholeString;
            wholeString.resize(stringSize);
            for(unsigned int cChar = 0; cChar < stringSize; ++cChar) //Read in the string from the bytecode into the allocated memory
                wholeString[cChar] = bytecode[++a];

            push_string(wholeString); //Push the resulting string
            break;
        }
        case Instruction::GOTO:
            a = bytecode[a+1]-1; //-1 because the bytecode position will increment after the loop ends
            break;
        case Instruction::CONSOLE_IN:
        {
            Type &variable = stack[stackSize - bytecode[++a] - 1];
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
                std::cin >> *variable.stringData;
                break;
            default:
                throwError(std::string("Failed to CONSOLE_IN, Unknown data type '" + std::to_string(variable.type) + "'"));
            }
            break;
        }
        case Instruction::MATH_ADD:
            {
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                int result = 0;
                for(unsigned int b = 0; b < numberCount; ++b)
                    result += pop().intData;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_SUBTRACT:
            {
                //TEMPORARY
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                std::vector<unsigned int> values;
                int result = 0;
                for(unsigned int a = 0; a < numberCount; ++a) //For the number of arguments specified, pop them all off the stack and subtract from 'result'
                    values.emplace_back(pop().intData);
                result = values.back();
                values.pop_back();
                for(auto iter = values.rbegin(); iter != values.rend(); ++iter)
                    result -= *iter;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_MULTIPLY:
            {
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                std::vector<unsigned int> values;
                int result = 0;
                for(unsigned int a = 0; a < numberCount; ++a) //For the number of arguments specified, pop them all off the stack and subtract from 'result'
                    values.emplace_back(pop().intData);
                result = values.back();
                values.pop_back();
                for(auto iter = values.rbegin(); iter != values.rend(); ++iter)
                    result *= *iter;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_DIVIDE:
            {
                //TEMPORARY
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                std::vector<unsigned int> values;
                int result = 0;
                for(unsigned int a = 0; a < numberCount; ++a) //For the number of arguments specified, pop them all off the stack and subtract from 'result'
                    values.emplace_back(pop().intData);
                result = values.back();
                values.pop_back();
                for(auto iter = values.rbegin(); iter != values.rend(); ++iter)
                    result /= *iter;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_MOD:
            {
                //TEMPORARY
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                std::vector<unsigned int> values;
                int result = 0;
                for(unsigned int a = 0; a < numberCount; ++a) //For the number of arguments specified, pop them all off the stack and subtract from 'result'
                    values.emplace_back(pop().intData);
                result = values.back();
                values.pop_back();
                for(auto iter = values.rbegin(); iter != values.rend(); ++iter)
                {
                    result %= *iter;
                }
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::CLONE_TOP:
            {
                push_type(stack[stackSize - bytecode[++a] - 1]); //Clone a variable from an OFFSET. If 0 is given, the top variable is taken, if 1 is given, the second to top variable is taken etc
                break;
            }
        case Instruction::CONCENTRATE_STRINGS:
            {
                //Pop strings off first that need to be concentrated
                unsigned int numberOfStrings = bytecode[++a];
                std::string stringBuffer;
                std::vector<std::string> poppedStrings;
                for(unsigned int a = 0; a < numberOfStrings; ++a)
                    poppedStrings.emplace_back(*pop().stringData);

                //Now add the strings to a buffer in reverse, as we need the first one to be first in the string, not last
                for(auto iter = poppedStrings.rbegin(); iter != poppedStrings.rend(); ++iter)
                    stringBuffer += *iter;

                //Push to stack
                push_string(stringBuffer);
                break;
            }
        case Instruction::CONDITIONAL_IF:
            {
                //Move bytecode offset to the one specified in the bytecode.
                //bytecode[a+1] = position to set if false
                if(!pop().boolData)
                {
                    a = bytecode[a+1];
                }
                else //Else move past the false position and continue running the bytecode
                {
                    a++;
                }
                break;
            }
        case Instruction::SET_VARIABLE:
            {
                stack[stackSize - bytecode[++a]] = pop(); //Get variable at the given offset and set it to the given value
                break;
            }
        case Instruction::COMPARE_EQUAL:
            {
                push_bool(isEqual(bytecode[++a])); //Push results of comparison. Taking argument count from next bytecode
                break;
            }
        case Instruction::COMPARE_UNEQUAL: //Compares last X things on the stack, returns true if they don't match
            {
                push_bool(!isEqual(bytecode[++a])); //Push results of comparison. Taking argument count from next bytecode
                break;
            }
        case Instruction::COMPARE_LESS_THAN:
            {
                ++a; //Skip number of things to compare, not currently used
                if(isLessThan(pop(), pop()))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_MORE_THAN:
            {
                ++a; //Skip number of things to compare, not currently used
                if(isMoreThan(pop(), pop()))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_MORE_THAN_OR_EQUAL:
            {
                ++a; //Skip number of things to compare, not currently used
                if(isMoreThanOrEqual(pop(), pop()))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_LESS_THAN_OR_EQUAL:
            {
                ++a; //Skip number of things to compare, not currently used
                if(isLessThanOrEqual(pop(), pop()))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_OR:
            {
                unsigned int compareCount = bytecode[++a]; //Number of things to compare
                bool orPassed = false;
                for(unsigned int b = 0; b < compareCount; b++)
                {
                    if(pop().boolData)
                    {
                        orPassed = true;
                        b = compareCount;
                    }
                }
                if(orPassed)
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::STACK_WALK:
            {
                stackSize -= bytecode[++a]; //Get the new position from the next byte
                break;
            }
        case Instruction::DYNAMIC_GOTO:
            {
                a = pop().intData-1; //-1 as a will increment after this loop ends
                break;
            }
        case Instruction::TO_INTEGER:
            {
                Type variable = pop();
                switch(variable.type)
                {
                case DataType::CHAR:
                        push_integer(variable.charData);
                    break;
                case DataType::STRING:
                        push_integer(atoi(variable.stringData->c_str()));
                    break;
                default:
                    throwError(std::string("Failed to TO_INTEGER, Unknown data type '" + std::to_string(variable.type) + "'"));
                }
                break;
            }
        case Instruction::TO_STRING:
            {
                Type variable = pop();
                switch(variable.type)
                {
                case DataType::INT:
                        push_string(std::to_string(variable.intData));
                    break;
                case DataType::CHAR:
                        push_string(std::to_string(variable.charData));
                    break;
                case DataType::BOOL:
                        push_string(std::to_string(variable.boolData));
                    break;
                case DataType::STRING:
                        push_string(*variable.stringData);
                    break;
                default:
                    throwError(std::string("Failed to TO_STRING, Unknown data type '" + std::to_string(variable.type) + "'"));
                }
                break;
            }
        default:
            throwError("Unknown instruction '" + std::to_string((int)bytecode[a]) + "'");
        }
    }

    auto endPoint = std::chrono::system_clock::now();
    std::cout << "\nTime elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count() << "ms";
}

void VirtualMachine::throwError(const std::string& reason)
{
    throw std::string(reason);
}
