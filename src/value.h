#pragma once
#include <stdint.h>
#include <string.h>

#include <iostream>
#include <string>
#include <exception>
using namespace std;

#include "json_type.h"
#include "json_object.h"
#include "json_array.h"

class json_object;
class json_array;
class value;

/*
    Value is the container class used by all json objects and arrays.
    This class allocates memory for and stores data that the json objects point to.
    Memory is automatically freed when the object is deleted.

    For syntax convenience, the '=' operator has been overloaded for the various types
    so that you can do something like value = input, and the object will assign memory for it.
    many of these objects are deep copy only, so there may be memory and performance issues
    when used extensively.
*/

//append to an existing string (useful in to_string() operations)
void operator+=(string& to_append, value& input);

class value
{
private:
    //this is the pointer where data is stored
    void* data;

    //erase memory and set the type to none
    //called by destructor and assignment operators (to clear the object's data)
    void clear();

    //helper functions for assignment and constructors
    //makes deep copies of all of these types
    bool* make_condition(bool input);
    double* make_decimal(double input);
    char* make_text(string input);
    int64_t* make_number(int64_t input);

public:
    //this tracks the type pointed to by the data object
    //pre-initialized to prevent any potential errors
    json_type type = json_type::none;

    //various constructors for possible types
    value();
    ~value();
    value(int input);
    value(bool input);
    value(double input);
    value(int64_t input);
    value(json_array* input);
    value(const char* input);
    value(json_object* input);

    //whole numbers
    value& operator=(int input);
    value& operator=(int64_t input);

    //decimal values
    value& operator=(float input);
    value& operator=(double input);

    //text
    value& operator=(string input);
    value& operator=(const char* input);

    //conditional(boolean)
    value& operator=(bool input);

    //json object
    value& operator=(json_object input); //deep copy is very slow, should be avoided where possible
    value& operator=(json_object* input);

    //json array
    value& operator=(json_array input); //deep copy is very slow, should be avoided where possible
    value& operator=(json_array* input);

    //copy functions
    value(const value& to_copy);
    value& operator=(const value& to_copy);

    //type checking functions
    bool is_int();
    bool is_bool();
    bool is_none();
    bool is_text();
    bool is_array();
    bool is_double();
    bool is_object();

    //typecasting functions
    int64_t      to_int();
    bool         to_bool();
    json_array*  to_array();
    double       to_double();
    json_object* to_object();
    //string is not required because to_string() already exists

    //casting overloads
    explicit operator bool()        const;
    explicit operator double()      const;
    explicit operator int64_t()     const;
    explicit operator string() const;

    //these are not explicit to simplify syntax
    operator json_array*();
    operator json_object*();

    value& operator[](const char* key);
    value& operator[](int index);

    //string operations
    string to_string();
    friend ostream& operator<<(ostream& os, value& input);
};