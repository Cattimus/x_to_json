#pragma once

#include "value.h"
#include "json_object.h"

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

class value;
class json_object;

class EXPORT json_array
{
private:
    //change this to map
    vector<value*> data;

public:
    json_array();
    ~json_array();
    
    //copy operations
    json_array(const json_array& to_copy);
    json_array& operator=(const json_array& to_copy);

    //access operators
    int count();
    value& operator[](int index);

    //hacky iteration
    const vector<value*>& items();
    
    //overrides to add new items
    void add(int to_add);
    void add(int64_t to_add);
    void add(const char* to_add);
    void add(double to_add);
    void add(bool to_add);
    void add(json_array* to_add);
    void add(json_object* to_add);
    void add(value* to_add);

    //string operations
    std::string to_string();
    friend ostream& operator<<(ostream& os, json_array& to_print);

};
