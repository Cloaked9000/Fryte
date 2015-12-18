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
            std::string wholeString;
            wholeString.resize(stringSize);
            for(unsigned int cChar = 0; cChar < stringSize; cChar++) //Read in the string from the bytecode into the allocated memory
                wholeString[cChar] = bytecode[++a];

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
                std::cin >> *variable.stringData;
                break;
            default:
                throwError(std::string("Failed to CONSOLE_IN, Unknown data type '" + std::to_string(variable.type) + "'"));
            }
            break;
        }
        case Instruction::MATH_ADD:
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
                    result += *iter;
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
                unsigned int numberCount = bytecode[++a]; //Get number of bytes to subtract from bytecode
                std::vector<int> values;
                int result = 0;
                for(unsigned int a = 0; a < numberCount; a++) //For the number of arguments specified, pop them all off the stack and subtract from 'result'
                    values.emplace_back(pop().intData);
                result = values.back();
                values.pop_back();
                for(auto iter = values.rbegin(); iter != values.rend(); iter++)
                    result *= *iter;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_DIVIDE:
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
                    result /= *iter;
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::MATH_MOD:
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
                {
                    result %= *iter;
                }
                push_integer(result); //Push the result to the stack
                break;
            }
        case Instruction::CLONE_TOP:
            {
                push_type(stack[bytecode[++a]]); //Clone a variable from a position in the stack to the top of the stack
                break;
            }
        case Instruction::CONCENTRATE_STRINGS:
            {
                //Pop strings off first that need to be concentrated
                unsigned int numberOfStrings = bytecode[++a];
                std::string stringBuffer;
                std::vector<std::string> poppedStrings;
                for(unsigned int a = 0; a < numberOfStrings; a++)
                    poppedStrings.emplace_back(*pop().stringData);

                //Now add the strings to a buffer in reverse, as we need the first one to be first in the string, not last
                for(auto iter = poppedStrings.rbegin(); iter != poppedStrings.rend(); iter++)
                    stringBuffer += *iter;

                //Push to stack
                push_string(stringBuffer);
                break;
            }
        case Instruction::COMPARE_EQUAL:
            {
                unsigned int compareCount = bytecode[++a]; //Number of things to compare
                std::vector<Type> thingsToCompare;
                for(unsigned int a = 0; a < compareCount; a++)
                    thingsToCompare.emplace_back(pop());
                push_bool(isEqual(thingsToCompare));
                break;
            }
        case Instruction::CONDITIONAL_IF:
            {
                //Move bytecode offset to the one specified in the bytecode.
                //bytecode[a+1] = position to set if false
                Type val = pop();
                if(val.type != DataType::BOOL)
                {
                    throwError(std::string("Can't convert type " + std::to_string(val.type) + " to boolean for comparison"));
                }
                if(!val.boolData)
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
                Type val = pop(); //Value to set it to
                unsigned int variableStackOffset = bytecode[++a]; //Find which variable to set
                stack[variableStackOffset] = val; //Update the value
                break;
            }
        case Instruction::COMPARE_UNEQUAL: //Compares last two things on the stack, returns true if they don't match
            {
                a++; //Skip number of things to compare, not currently used

                if(!isEqual({pop(), pop()}))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_LESS_THAN:
            {
                a++; //Skip number of things to compare, not currently used

                const Type &v1 = pop(), v2 = pop();
                if(!isLessThan(v1, v2) && !isEqual({v1, v2}))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_MORE_THAN:
            {
                a++; //Skip number of things to compare, not currently used
                const Type &v1 = pop(), v2 = pop();
                if(isLessThan(v1, v2) && !isEqual({v1, v2}))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_MORE_THAN_OR_EQUAL:
            {
                a++; //Skip number of things to compare, not currently used

                const Type &v1 = pop(), v2 = pop();
                if(isLessThan(v1, v2) || isEqual({v1, v2}))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_LESS_THAN_OR_EQUAL:
            {
                a++; //Skip number of things to compare, not currently used
                const Type &v1 = pop(), v2 = pop();
                if(!isLessThan(v1, v2) || isEqual({v1, v2}))
                    push_bool(true);
                else
                    push_bool(false);
                break;
            }
        case Instruction::COMPARE_OR:
            {
                unsigned int compareCount = bytecode[++a]; //Number of things to compare
                bool wasFound = false;
                //Iterate through each thing to compare and push true if any of the values are true then break
                for(unsigned int a = 0; a < compareCount && !wasFound; a++)
                {
                    const Type &current = pop();
                    if(current.type != DataType::BOOL)
                        throwError("Invalid OR comparison, not boolean!");
                    if(current.boolData)
                    {
                        push_bool(true);
                        wasFound = true;
                    }
                }
                if(!wasFound)
                {
                    //False otherwise
                    push_bool(false);
                }
                break;
            }
        default:
            throwError("Unknown instruction '" + std::to_string(currentInstruction) + "'");
        }
    }
}

bool VirtualMachine::isLessThan(const Type& v1, const Type& v2)
{
    //Ensure that they're the same type or the union comparison will screw up
    if(v2.type != v1.type)
    {
        throwError("Can't compare different data types!");
    }

    //Compare different things depending on the variable types
    switch(v1.type)
    {
    case INT:
        if(v1.intData < v2.intData)
            return true;
        else
            return false;
        break;
    case CHAR:
        if(v1.charData < v2.charData)
            return true;
        else
            return false;
        break;
    case STRING:
        if(v1.stringData->size() < v2.stringData->size())
            return true;
        else
            return false;
        break;
    case BOOL:
        if(v1.boolData < v2.boolData)
            return true;
        else
            return false;
    default:
        throwError("Internal error, attempt to compare unknown data type");
    }
    throwError("Internal error, isEqual comparison failed for unknown reason");
    return false;
}

bool VirtualMachine::isEqual(const std::vector<Type> &vals)
{
    //Ensure that they're the same type or the union comparison will screw up
    DataType commonType = vals[0].type;
    for(unsigned int a = 1; a < vals.size(); a++)
    {
        if(vals[a].type != commonType)
            throwError("Can't compare different data types");
    }

    //Compare the first value against the rest
    for(unsigned int a = 1; a < vals.size(); a++)
    {
        if(!compare(vals[0], vals[a])) //If not matching
            return false;
    }
    return true;
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

void VirtualMachine::push_string(const std::string &value)
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

bool VirtualMachine::compare(const Type &v1, const Type &v2)
{
    //Compare different things depending on the variable types
    switch(v1.type)
    {
    case INT:
        if(v1.intData == v2.intData)
            return true;
        else
            return false;
        break;
    case CHAR:
        if(v1.charData == v2.charData)
            return true;
        else
            return false;
        break;
    case STRING:
        if(*v1.stringData == *v2.stringData)
            return true;
        else
            return false;
        break;
    case BOOL:
        if(v1.boolData == v2.boolData)
            return true;
        else
            return false;
    default:
        return false;
    }
    return false;
}
