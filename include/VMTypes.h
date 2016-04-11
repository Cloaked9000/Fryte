#ifndef VMTYPES_H
#define VMTYPES_H

//List of instructions which the virtual machine supports
enum Instruction
{
    CONSOLE_OUT = 0, //Prints out value, CONSOLE_OUT(data)
    CREATE_INT = 1, //Pushes an integer to the stack, CREATE_INT(data)
    CREATE_CHAR = 2, //Pushes a character to the stack, CREATE_CHAR(data)
    CREATE_BOOL = 3, //Pushes a boolean to the stack, CREATE_BOOL(data)
    CREATE_STRING = 4, //Pushes a string to the stack, CREATE_STRING(length, data1, data2, etc)
    GOTO = 5, //Sets the current bytecode position, GOTO(stackDepth)
    CONSOLE_IN = 6, //Reads in a value, CONSOLE_IN(data)
    MATH_ADD = 7, //Adds two or more numbers. MATH_ADD(NumberOfThingsToAdd, data1, data2, etc)
    MATH_SUBTRACT = 8, //Subtracts two or more numbers. MATH_SUBTRACT(NumberOfThingsToAdd, data1, data2, etc)
    MATH_MULTIPLY = 9, //Multiplies two or more numbers together. MATH_MULTIPLY(NumberOfThingsToAdd, data1, data2, etc)
    MATH_DIVIDE = 10, //Divides two or more numbers. MATH_DIVIDE(NumberOfThingsToAdd, data1, data2, etc)
    MATH_MOD = 11, //Modulus two or more things. MATH_MOD(NumberOfThingsToAdd, data1, data2, etc)
    CLONE_TOP = 12, //Clones a variable's contents from a position in the stack to the top of the stack. CLONE_TOP(StackPos)
    CONCENTRATE_STRINGS = 13, //Concentrates strings together into a new string. CONCENTRATE_STRINGS(NumberOfStrings)
    COMPARE_EQUAL = 14, //Compare the last two things on the stack and add true or false if they are equal
    CONDITIONAL_IF = 15, //If the last thing on the stack is 1, moves bytecode position to next, else moves bytecode position to next+1
    SET_VARIABLE = 16, //Sets the data of a variable in the stack. SET_VARIABLE(stackPos). New value taken from top of stack.
    COMPARE_UNEQUAL = 17, //Compare the last two things on the stack and add true or false if they are unequal
    COMPARE_LESS_THAN = 18, //Compare last two things on the stack and push true if object one is less than object two
    COMPARE_MORE_THAN = 19, //Compare last two things on the stack and push true if object one is more than object two
    COMPARE_LESS_THAN_OR_EQUAL = 20, //Compare last two things on the stack and pushes true if object one is less than or equal to object two
    COMPARE_MORE_THAN_OR_EQUAL = 21, //Compare last two things on the stack and pushes true if object one is more than or equal to object two
    COMPARE_OR = 22, //Compare a series of values and return true if one of them is true. False otherwise. COMPARE_OR(numberOfThings, v1, v2, v3...)
    STACK_WALK = 23, //Sets the current stack size, STACK_WALK(pos)
    DYNAMIC_GOTO = 24, //Sets the bytecode to a position which is found on top of the stack. DYNAMIC_GOTO(pos)
    TO_INTEGER = 25, //Converts the value on the top of the stack to a string and pushes the new value
    TO_STRING = 26, //Converts the value on the top of the stack to an integer and then pushes the new value
    COMPARE_AND = 27, //Compare a series of values and return true if all values are true. False otherwise. COMPARE_AND(ArgCount, Args...)
    DYNAMIC_CLONE_TOP = 28, //Takes an offset from the top of the stack and clones the variable at that offset to the top. DYNAMIC_CLONE_TOP(
    DYNAMIC_SET_VARIABLE = 29, //Takes a variable position and variable value from the top of the stack and sets that variable to that value. DYNAMIC_SET_VARIABLE(VALUE, POSITION)
};

//List of data types which the virtual machine supports
enum DataType
{
    INT = 0,
    CHAR = 1,
    BOOL = 2,
    STRING = 3,
};

//A single data type stored in the stack
struct Type
{
    Type() noexcept //Default constructor
    {
        stringData = nullptr;
    }
    virtual ~Type() noexcept
    {
        if(type == DataType::STRING && stringData != nullptr)
            delete stringData;
    }

    Type(unsigned int data) noexcept //Integer constructor
    : intData(data), type(DataType::INT){}

    Type(bool data) noexcept //Boolean constructor
    : boolData(data), type(DataType::BOOL){}

    Type(unsigned char data) noexcept //Character constructor
    : charData(data), type(DataType::CHAR){}

    Type(const std::string &data) noexcept //String constructor
    : stringData(new std::string(data)), type(DataType::STRING){}

    Type(const Type &other) noexcept //Copy constructor
    : intData(other.intData), type(other.type)
    {
        if(type == DataType::STRING)
            stringData = new std::string(*other.stringData);
    }

    Type(Type &&other) noexcept //Move constructor
    {
        type = other.type;
        if(other.type == DataType::STRING)
        {
            stringData = other.stringData;
            other.stringData = nullptr;
        }
        else
        {
            intData = other.intData;
        }
    }

    void operator=(const Type &other) noexcept //Assignment constructor
    {
        type = other.type;
        intData = other.intData;
        if(type == DataType::STRING)
            stringData = new std::string(*other.stringData);
    }

    union //Stores the data itself
    {
        unsigned int intData;
        bool boolData;
        unsigned char charData;
        std::string *stringData;
    };

    DataType type; //Store the type of the variable
};

#endif // VMTYPES_H
