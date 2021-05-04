#pragma once

//all of the files in our project
#include "value.h"
#include "json_type.h"
#include "json_array.h"
#include "json_object.h"

#include <iostream>
using namespace std;

//MSVC DLL export strangeness
#ifdef _MSC_VER
    #ifdef LIBRARY_EXPORT
        #define EXPORT __declspec(dllexport)
    #else
        #define EXPORT __declspec(dllimport)
    #endif
#else
    #define EXPORT
#endif


//TODO - This file in particular has security concerns. Could use a partial or full rewrite in the future

//forward declarations (to prevent circular issues)
class json_object;
class json_array;
class value;

//change json string to human-readable format
//indentation seems to fail with very large strings, requires investigation
EXPORT string beautify_json(string to_beautify);

//create objects
EXPORT json_object* create_object(string to_parse);
EXPORT json_array* create_array(string to_parse);

//create a value from string
value* create_value(string to_parse);

//main parser function
//rework to fail if the json is not valid
//also account for escaped quotes in strings
vector<string> split_json(string to_parse);

//helper function for create_object
vector<string> handle_keypair(string to_parse);
