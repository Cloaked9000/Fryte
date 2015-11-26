#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <iostream>

//List of instructions which the virtual machine supports
enum Instruction
{
    CONSOLE_OUT = 0,
    CREATE_INT = 1,
    CREATE_CHAR = 2,
    CREATE_BOOL = 3,
    CREATE_STRING = 4,
};

//List of data types which the virtual machine supports
enum DataType
{
    INT,
    CHAR,
    BOOL,
    STRING,
};

//A single data type stored in the stack
struct Type
{
    DataType type; //Store the type of the variable

    Type() //Default constructor
    {

    }

    Type(int data) //Integer constructor
    {
        intData = data;
        type = DataType::INT;
    }

    Type(unsigned char data) //Character constructor
    {
        charData = data;
        type = DataType::CHAR;
    }

    Type(bool data) //Boolean constructor
    {
        boolData = data;
        type = DataType::BOOL;
    }

    Type(char *data) //String constructor
    {
        stringData = data;
        type = DataType::STRING;
    }

    union //Stores the data itself
    {
        int intData;
        bool boolData;
        char charData;
        char *stringData;
    };
};

class VirtualMachine
{
    public:
        /** Default constructor */
        VirtualMachine();
        /** Default destructor */
        virtual ~VirtualMachine();

        void interpret(char bytecode[], unsigned int byteSize); //Executes an array of bytecode

        void push_integer(int value); //Pushes an integer onto the stack
        void push_char(unsigned char value); //Pushes a character onto the stack
        void push_bool(bool value); //Pushes a boolean value onto the stack
        void push_string(char *value); //Pushes a string onto the stack

        Type pop(); //Pops a value off the stack
    protected:
    private:
        void popStackCheck(); //Checks that a value CAN be popped off, if not, throws an error
        void pushStackCheck(); //Checks that a value CAN be pushed to the stack, if not, throw an error

        static const unsigned int maxStackSize = 128; //Maximum stack size
        unsigned int stackSize; //Current stack position
        Type stack[maxStackSize]; //The stack itself
};

#endif // VIRTUALMACHINE_H
