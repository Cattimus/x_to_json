#include "x_to_json.h"

//adds indentations and newlines to single line json
string beautify_json(string to_beautify)
{
    string to_return;
    char prev;
    bool in_quote = false;
    int indentation_level = 0;
    for (auto cur : to_beautify)
    {   
        //copy everything in quotes, ignore all characters until out of quote
        if (in_quote)
        {
            if(cur == '"' && prev != '\\')
            {
                in_quote = !in_quote;
            }

            to_return += cur;
        }
        else
        {
            
            //apply formatting rules based on the content
            switch (cur)
            {
            
            //indent
            case '{':
                to_return += "{\n";
                indentation_level++;
                indent(indentation_level, &to_return);
                break;
            case '[':
                to_return += "[\n";
                indentation_level++;
                indent(indentation_level, &to_return);
                break;
            case ',':
                to_return += ",\n";
                indent(indentation_level, &to_return);
                break;

            //remove indent
            case ']':
                indentation_level--;
                if (prev == '[')
                {
                    remove_indent(&to_return);
                    to_return += "]";
                }
                else
                {
                    to_return += "\n";
                    indent(indentation_level, &to_return);
                    to_return += cur;
                }
                break;
            case '}':
                indentation_level--;
                if (prev == '{')
                {
                    remove_indent(&to_return);
                    to_return += '}';
                }
                else
                {
                    to_return += "\n";
                    indent(indentation_level, &to_return);
                    to_return += cur;
                }
                break;
            
            //for detecting quotes
            case '"':
                in_quote = !in_quote;
                to_return += cur;
                break;

            //minor syntax change to improve readability
            case ':':
                to_return += ": ";
                break;

            default:
                to_return += cur;
                break;
            }
        }
        prev = cur;
    }
    return to_return;
}

//accepts a json string (formatted or unformatted), and creates a json object
json_object* create_object(string to_parse)
{
    json_object* to_return = new json_object();

    //empty json object (return nothing essentially)
    if(to_parse.length() <= 2)
    {
        return to_return;
    }

    //parse
    string sub = to_parse.substr(1, to_parse.length() - 2);
    vector<string> keypairs = split_json(sub);
    for(auto i : keypairs)
    {
        vector<string> values = handle_keypair(i);
        to_return->add(values[0].c_str(), create_value(values[1]));
    }

    return to_return;
}
json_array* create_array(string to_parse)
{
    json_array* to_return = new json_array();

    if(to_parse.length() <= 2)
    {
        return to_return;
    }

    string sub = to_parse.substr(1, to_parse.length() - 2);
    vector<string> values = split_json(sub);
    for(auto i : values)
    {
        to_return->add(create_value(i));
    }

    return to_return;
}

//creates a new keypair object based on the input line, assumed output from split_json
vector<string> handle_keypair(string to_parse)
{
    vector<string> to_return;
    string val;

    //split on ":" and assign values
    int key_index = to_parse.find(':');
    to_return.push_back(to_parse.substr(1, key_index - 2));
    to_return.push_back(to_parse.substr(key_index + 1, to_parse.length()));

    return to_return;
}


//Create a value given a string
value* create_value(string val)
{
    value* to_return;

    //json object
    if(val[0] == '{')
    {
        json_object* temp = create_object(val);
        to_return = new value(temp);
        return to_return;
    }

    //json array
    if(val[0] == '[')
    {
        json_array* temp = create_array(val);
        to_return = new value(temp);
        return to_return;
    }

    //value contains text
    if(val[0] == '"')
    {
        string temp = val.substr(1, val.length() - 2);
        to_return = new value(temp.c_str());
        return to_return;
    }

    //value contains bool
    if(val == "true" || val == "True")
    {
        to_return = new value(true);
        return to_return;
    } 
    if(val == "false" || val == "False")
    {
        to_return = new value(false);
        return to_return;
    }

    //number(int64_t)
    if(is_num(val))
    {
        to_return = new value((int64_t)stoll(val));
        return to_return;
    }

    //decimal(double)
    if(is_decimal(val))
    {
        to_return = new value(stod(val));
        return to_return;
    }

    //default case
    to_return = new value();
    return to_return;
}

//check if string contains only numbers
bool is_num(string input)
{
    for(auto i : input)
    {
        if(i - '0' < 0 || i - '0' > 9)
        {
            return false;
        }
    }

    return true;
}

//check if string is a decimal value(numbers and 1 singular period)
bool is_decimal(string input)
{
    int decimal_counter = 0;
    for(auto i : input)
    {
        if(((i - '0' < 0) || (i - '0' > 9)) && (i != '.'))
        {
            return false;
        }

        if(i == '.')
        {
            decimal_counter++;
            if(decimal_counter > 0)
            {
                return false;
            }
        }
    }

    if(decimal_counter < 1)
    {
        return false;
    }

    return true;
}

//TO-DO - Fail on invalid json
//parses json into lines
vector<string> split_json(string to_parse)
{
    vector<string> to_return;
    
    bool is_object = false;
    bool is_quote = false;
    char prev = 0;

    int embed_level = 0;

    string current_line;
    for(auto cur : to_parse)
    {
        //all data between quotes is copied directly
        if(is_quote)
        {
            //escaped quotes shouldn't exit the quote block
            if(cur == '"' && prev != '\\')
            {
                is_quote = false;
            }
            current_line += cur;
            prev = cur;
            continue;
        }

        //objects should be copied without being split
        if(is_object)
        {
            //objects may have other objects embedded within
            if(cur == '{' || cur == '[')
            {
                embed_level++;
            }
            if(cur == '}' || cur == ']')
            {
               embed_level--;

               if(embed_level == 0)
               {
                   is_object = false;
               } 
            }

            //check for quotes within the object
            if(cur == '\"')
            {
                is_quote = true;
            }

            if(!isspace(cur))
            {
                current_line += cur;
            }
            continue;
        }

        if(cur == '{' || cur == '[')
        {
            is_object = true;
            embed_level++;
        }

        if(cur == '\"')
        {
            is_quote = true;
        }

        //this is the character we're splitting on
        //each split will be appended to the vector
        if(cur == ',' && !is_quote && !is_object)
        {
            to_return.push_back(current_line);
            current_line.clear();
            continue;
        }

        if(!isspace(cur))
        {
            current_line += cur;
        }
    }

    to_return.push_back(current_line);

    return to_return;
}

//helper function for beautify
void indent(int level, string* to_indent)
{
    for(int i = 0; i < level; i++)
    {
        *to_indent += "    ";
    }
}

//helper function for beautify
void remove_indent(string* to_indent)
{
    char temp = 0;
    while(temp != '\n')
    {
        temp = (*to_indent).back();
        (*to_indent).pop_back();
    }
}