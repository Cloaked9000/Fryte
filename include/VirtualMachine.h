#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "VMTypes.h"

class VirtualMachine
{
public:
    /** Default constructor */
    VirtualMachine();
    /** Default destructor */
    virtual ~VirtualMachine();

    void interpret(unsigned int bytecode[], int byteSize); //Executes an array of bytecode

    inline void push_integer(int value)
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
    bool isEqual(const std::vector<Type> &vals);
    bool compare(const Type &v1, const Type &v2); //Compares two values, returns true if they're equal, false otherwise. Throws error

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
