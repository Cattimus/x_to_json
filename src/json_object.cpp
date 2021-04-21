#include "json_object.h"

json_object::json_object()
{
    return;
}

//parse string and assign values to json object
json_object::json_object(string to_parse)
{
    //empty json object (return nothing essentially)
    if(to_parse.length() <= 2)
    {
        return;
    }

    string sub = to_parse.substr(1, to_parse.length() - 2);
    vector<string> keypairs = split_json(sub);
    for(auto i : keypairs)
    {
        vector<string> values = handle_keypair(i);
        this->add(values[0].c_str(), create_value(values[1]));
    }
}

json_object::~json_object()
{
    for(auto i : data)
    {
        delete i.second;
    }
}

bool json_object::has(string key)
{
    return(!(data.find(key) == data.end()));
}

const map<string, value*>& json_object::items()
{
    return data;
}

json_object::json_object(const json_object& to_copy)
{
    *this = to_copy;
}

json_object& json_object::operator=(const json_object& to_copy)
{
    for(auto i : to_copy.data)
    {
        data[i.first] = new value(*i.second);
    }

    return *this;
}

//access values and create new ones if none exist
value& json_object::operator[](string index)
{
    value* to_return = NULL;
    to_return = data[index];

    if(to_return == NULL)
    {
        to_return = new value();
        data[index] = to_return;
    }

    return *to_return;
}

string json_object::to_string()
{
    string to_return = "{";
    for(auto i : data)
    {
        if(i.second->is_text())
        {
            to_return += '"' + i.first + '"' + ":" + '"' + i.second->to_string() + '"' + ",";
        }
        else
        {
            to_return += '"' + i.first + '"' + ":" + i.second->to_string() + ",";
        }
    }
    
    if(data.size() >=1)
    {
        to_return.pop_back();
    }

    to_return += "}";
    return to_return;
}

void json_object::add(string key, value* object)
{
    data[key] = object;
}

ostream& operator<<(ostream& os, json_object& to_print)
{
    return os << to_print.to_string();
}