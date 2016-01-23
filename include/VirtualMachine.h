#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <iostream>
#include <vector>
#include <chrono>
#include <cstring>

#include "VMTypes.h"

class VirtualMachine
{
public:
    /** Default constructor */
    VirtualMachine();
    /** Default destructor */
    virtual ~VirtualMachine();

    void interpret(unsigned int bytecode[], int byteSize); //Executes an array of bytecode

    inline void push_integer(unsigned int value)
    {
        stack[stackSize++] = Type(value);
    }

    inline void push_char(unsigned char value)
    {
        stack[stackSize++] = Type(value);
    }

    inline void push_bool(bool value)
    {
        stack[stackSize++] = Type(value);
    }

    inline void push_string(const std::string &value)
    {
        stack[stackSize++] = Type(value);
    }

    inline void push_type(Type value)
    {
        stack[stackSize++] = value;
    }

    inline Type pop()
    {
        return stack[--stackSize];
    }

protected:
private:
    void popStackCheck(); //Checks that a value CAN be popped off, if not, throws an error
    void pushStackCheck(); //Checks that a value CAN be pushed to the stack, if not, throw an error
    void throwError(const std::string &reason); //Throws an error, this function adds additional information to the reason
    inline bool isEqual(unsigned int compareCount)
    {
        Type &&val1 = pop();
        for(unsigned int b = 1; b < compareCount; ++b)
        {
            if(!compare(val1, pop()))
            {
                return false;
            }
        }
        return true;
    }

    inline bool compare(const Type &v1, const Type &v2)
    {
        switch(v1.type)
        {
        case INT:
            if(v1.intData == v2.intData)
                return true;
            break;
        case CHAR:
            if(v1.charData == v2.charData)
                return true;
            break;
        case STRING:
            if(*v1.stringData == *v2.stringData)
                return true;
            break;
        case BOOL:
            if(v1.boolData == v2.boolData)
                return true;
        default:
            return false;
        }
        return false;
    }

    inline bool isLessThan(const Type& v1, const Type& v2)
    {
        return (v1.intData < v2.intData);
    }

    inline bool isMoreThan(const Type& v1, const Type& v2)
    {
        return (v1.intData > v2.intData);
    }

    inline bool isLessThanOrEqual(const Type& v1, const Type& v2)
    {
        return (v1.intData <= v2.intData);
    }

    inline bool isMoreThanOrEqual(const Type& v1, const Type& v2)
    {
        return (v1.intData >= v2.intData);
    }

    static const unsigned int maxStackSize = 500; //Maximum stack size
    unsigned int stackSize; //Current stack position
    Type stack[maxStackSize]; //The stack itself
};

#endif // VIRTUALMACHINE_H
