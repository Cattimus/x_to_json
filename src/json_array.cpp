#include "json_array.h"

json_array::json_array()
{
    return;
}

json_array::json_array(const json_array& to_copy)
{
    *this = to_copy;
}

//deep copy another json_array (can be VERY slow if overused)
//it is not intended to repeatedly deep copy json arrays
json_array& json_array::operator=(const json_array& to_copy)
{
    for(auto it : to_copy.data)
    {
        data.push_back(new value(*it));
    }
    return *this;
}

//this should automatically free all dynamically allocated memory
json_array::~json_array()
{
    for(auto it : data)
    {
        delete it;
    }
}

int json_array::count()
{
    return data.size();
}

value& json_array::operator[](int index)
{
    return *data[index];
}

//add to array functions
void json_array::add(int to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(int64_t to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(const char* to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(double to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(bool to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(json_array* to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(json_object* to_add)
{
    data.push_back(new value(to_add));
}
void json_array::add(value* to_add)
{
    data.push_back(to_add);
}

//for cheaty iteration
const vector<value*>& json_array::items()
{
    return data;
}

//string operations
std::string json_array::to_string()
{
    string to_return = "[";
    for(auto i : data)
    {
        to_return += i->to_string() + ",";
    }

    if(data.size() >=1)
    {
        to_return.pop_back();
    }

    to_return += "]";

    return to_return;
}
ostream& operator<<(ostream& os, json_array& to_print)
{
    return os << to_print.to_string();
}