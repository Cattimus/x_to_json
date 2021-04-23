#include "value.h"

value::value()
{
    type = json_type::none;
}

value::~value()
{
    clear();
}

void value::clear()
{
    //do nothing if the object is not initialized
    if(type == json_type::none)
    {
        return;
    }

    //clear the data allocated by malloc if the data has been set by malloc
    if(type >= json_type::number && type <= json_type::condition)
    {
        free(data);
        data = NULL;
        type = json_type::none;
        return;
    }

    //clear data that has been allocated by C++'s "new" operator
    if(type == json_type::object)
    {
        delete (json_object*)data;
        data = NULL;
        type = json_type::none;
        return;
    }

    if(type == json_type::array)
    {
        delete (json_array*)data;
        data = NULL;
        type = json_type::none;
        return;
    }
}

//constructors for primitive types
value::value(int64_t input)
{
    data = make_number(input);
    type = json_type::number;
}
value::value(int input)
{
    data = make_number((int64_t)input);
    type = json_type::number;
}
value::value(double input)
{
    data = make_decimal(input);
    type = json_type::decimal;
}
value::value(const char* input)
{
    data = make_text(string(input));
    type = json_type::text;
}
value::value(bool input)
{
    data = make_condition(input);
    type = json_type::condition;
}

//assignment operator overloads for primitive types
value& value::operator=(int64_t input)
{
    clear();
    data = make_number(input);
    type = json_type::number;
    return *this;
}
value& value::operator=(int input)
{
    clear();
    data = make_number((int64_t)input);
    type = json_type::number;
    return *this;
}
value& value::operator=(float input)
{
    clear();
    data = make_decimal((double)input);
    type = json_type::decimal;
    return *this;
}
value& value::operator=(double input)
{
    clear();
    data = make_decimal(input);
    type = json_type::decimal;
    return *this;
}
value& value::operator=(std::string input)
{
    clear();
    data = make_text(input);
    type = json_type::text;
    return *this;
}
value& value::operator=(const char* input)
{
    clear();
    data = make_text(std::string(input));
    type = json_type::text;
    return *this;
}
value& value::operator=(bool input)
{
    clear();
    data = make_condition(input);
    type = json_type::condition;
    return *this;
}

//helper functions for constructors and = operators
int64_t* value::make_number(int64_t input)
{
    int64_t* to_return = (int64_t*)calloc(1, sizeof(int64_t));
    *to_return = input;
    return to_return;
}
double* value::make_decimal(double input)
{
    double* to_return = (double*)calloc(1, sizeof(double));
    *to_return = input;
    return to_return;
}
char* value::make_text(std::string input)
{
    char* to_return = (char*)calloc(input.length() + 1, sizeof(char));
    strncpy(to_return, input.c_str(), input.length());
    return to_return;
}
bool* value::make_condition(bool input)
{
    bool* to_return = (bool*)calloc(1, sizeof(bool));
    *to_return = input;
    return to_return;
}

//constructor and assigment overrides for json_object
value::value(json_object* input)
{
    data = input;
    type = json_type::object;
}

//deep copy for json_object
//potentially VERY slow, use with caution
value& value::operator=(json_object input)
{
    clear();
    json_object* temp = new json_object();
    *temp = input;
    data = temp;
    type = json_type::object;
    return *this;
}

//recommended way to assign json_object values
value& value::operator=(json_object* input)
{
    clear();
    data = input;
    type = json_type::object;
    return *this;
}

//constructor and assignment operators for json_array object
value::value(json_array* input)
{
    data = input;
    type = json_type::array;
}

//same advice as json_object, potentially very slow
//use json_array* wherever possible
value& value::operator=(json_array input)
{
    clear();
    json_array* temp = new json_array;
    *temp = input;
    data = temp;
    type = json_type::array;
    return *this;
}
value& value::operator=(json_array* input)
{
    clear();
    data = input;
    type = json_type::array;
    return *this;
}

value::value(const value& to_copy)
{
    *this = to_copy;
}
//potentially very slow depending on which operations are used
//by it's nature this has to be deep copy, which leads to potential slowdown
value& value::operator=(const value& to_copy)
{   
    switch(to_copy.type)
    {
        case json_type::none:
            type = json_type::none;
            return *this;
            break;

        case json_type::number:
            *this = *((int64_t*)to_copy.data);
            return *this;
            break;

        case json_type::decimal:
            *this = *((double*)to_copy.data);
            return *this;
            break;

        case json_type::condition:
            *this = *((bool*)to_copy.data);
            return *this;
            break;

        case json_type::text:
            *this = *((const char*)to_copy.data);
            return *this;
            break;

        //these are the source of potentially slow operations
        case json_type::object:
            *this = *(json_object*)to_copy.data;
            return *this;
            break;
        case json_type::array:
            *this = *(json_array*)to_copy.data;
            return *this;
            break;

        //default case should never be hit, if it has something has gone wrong
        default:
            clear();
            return *this;
            break;
    }
}

//convert the object held by container to string depending on type
string value::to_string()
{
    switch(type)
    {
        case json_type::none:
            return "null";
            break;

        case json_type::number:
            return std::to_string(*(int64_t*)data);
            break;

        case json_type::decimal:
            return std::to_string(*(double*)data);
            break;

        case json_type::text:
            return string((char*)data);
            break;

        case json_type::condition:
            if(*(bool*)data)
            {
                return "true";
            }
            
            return "false";
            break;

        case json_type::object:
            return (*(json_object*)data).to_string();
            break;

        case json_type::array:
            return (*(json_array*)data).to_string();
            break;

        //this case should never be hit
        default:
            return "This is a critical error. Value class has not been initialised or has been corrupted. Type: " + type;
            break;
    }
}

std::ostream& operator<<(std::ostream& os, value& input)
{
    return os << input.to_string();
}

//this is a string append operator which is used primarily to simplify syntax
void operator+=(string& to_append, value& input)
{
    to_append += input.to_string();
}

//type checking functions
bool value::is_int()
{
    return (type == json_type::number);
}
bool value::is_text()
{
    return (type == json_type::text);
}
bool value::is_bool()
{
    return (type == json_type::condition);
}
bool value::is_double()
{
    return (type == json_type::decimal);
}
bool value::is_object()
{
    return (type == json_type::object);
}
bool value::is_array()
{
    return (type == json_type::array);
}
bool value::is_none()
{
    return (type == json_type::none);
}

//casting functions (to simplify syntax)
int64_t value::to_int()
{
    return *(int64_t*)data;
}
double value::to_double()
{
    return *(double*)data;
}
bool value::to_bool()
{
    return *(bool*)data;
}
json_array* value::to_array()
{
    return (json_array*)data;
}
json_object* value::to_object()
{
    return (json_object*)data;
}

//casting operators must be overloaded since we're using void*
//misuse will result in an exception throw
value::operator int64_t() const
{
    if(type != json_type::number)
    {
        throw invalid_argument("Cast does not match value object's internal type.");
    }

    return *(int64_t*)data;
}
value::operator std::string() const
{
    if(type != json_type::text)
    {
        throw invalid_argument("Cast does not match value object's internal type.");
    }
    return string((const char*)data);
}
value::operator double() const 
{
    if(type != json_type::decimal)
    {
        throw invalid_argument("Cast does not match value object's internal type.");
    }
    return *(double*)data;
}
value::operator bool() const
{
    if(type != json_type::condition)
    {
        throw invalid_argument("Cast does not match value object's internal type.");
    }

    return *(bool*)data;
}
value::operator json_object*()
{
    if(type != json_type::object)
    {
        throw invalid_argument("Cast does not match value object's internal type.");
    }

    return (json_object*)data; 
}
value::operator json_array*()
{
    if(type != json_type::array)
    {
        throw invalid_argument("Cast does not match value object's internal type.");
    }

    return (json_array*)data;
}

value& value::operator[](const char* key)
{
    if(type == json_type::object)
    {
        return (*(json_object*)data)[key];
    }
    else
    {
        //this is to inform the user what exactly went wrong
        string to_return = "Invalid argument exception. Internal type is ";
        
        switch(type)
        {
            case json_type::number:
                to_return += "number";
                break;
            
            case json_type::array:
                to_return += "json_array";
                break;

            case json_type::condition:
                to_return += "boolean";
                break;

            case json_type::text:
                to_return += "text";
                break;
            
            case json_type::none:
                to_return += "uninitialized";
                break;

            case json_type::decimal:
                to_return += "decimal";
                break;

            //this case will never be hit but it would make a pretty hilarious error if it was
            case json_type::object:
                to_return += "json_object";
                break;
        }

        to_return += ", not json_object";
        throw invalid_argument(to_return);
    }
}

value& value::operator[](int index)
{
    if(type == json_type::array)
    {
        return (*(json_array*)data)[index];
    }
    else
    {
        //this is to inform the user what exactly went wrong
        string to_return = "Invalid argument exception. Internal type is ";
        
        switch(type)
        {
            case json_type::number:
                to_return += "number";
                break;
            
            case json_type::object:
                to_return += "json_object";
                break;

            case json_type::condition:
                to_return += "boolean";
                break;

            case json_type::text:
                to_return += "text";
                break;
            
            case json_type::none:
                to_return += "uninitialized";
                break;

            case json_type::decimal:
                to_return += "decimal";
                break;

            //this case will never be hit but it would make a pretty hilarious error if it was
            case json_type::array:
                to_return += "json_array";
                break;
        }

        to_return += ", not json_array";
        throw invalid_argument(to_return);
    }
}
