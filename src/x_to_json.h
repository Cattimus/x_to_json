#pragma once

//all of the files in our project
#include "value.h"
#include "json_type.h"
#include "json_array.h"
#include "json_object.h"

#include <iostream>
using namespace std;

//TODO - This file in particular has security concerns. Could use a partial or full rewrite in the future

//forward declarations (to prevent circular issues)
class json_object;
class json_array;
class value;

//change json string to human-readable format
//indentation seems to fail with very large strings, requires investigation
string beautify_json(string to_beautify);
void indent(int level, string* to_indent);
void remove_indent(string* to_indent);

//create objects
json_object* create_object(string to_parse);
json_array*  create_array(string to_parse);

//create a value from string
value* create_value(string to_parse);
bool is_num(string input);
bool is_decimal(string input);

//main parser function
//rework to fail if the json is not valid
//also account for escaped quotes in strings
vector<string> split_json(string to_parse);

//helper function for create_object
vector<string> handle_keypair(string to_parse);