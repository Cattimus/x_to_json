#pragma once
#include<vector>
#include<iostream>
using namespace std;

#include "x_to_json.h"
#include <map>

class value;
class json_array;

class json_object
{
private:
    map<string, value*> data;

    //helper function to create json object from string without copying
    void create_object(string to_parse);

public:
    //constructors
    json_object();
    json_object(string input);
    ~json_object();

    //object copy
    json_object(const json_object& to_copy);
    json_object& operator=(const json_object& to_copy);

    //hacky terrible way to make this iteratable
    const map<string, value*>& items();

    //check if json_object has a specific key
    bool has(string key);

    //getter and setter
    value& operator[](string index);
    void add(string key, value* object);

    //string operations
    string to_string();
    friend ostream& operator<<(ostream& os, json_object& to_print);
};