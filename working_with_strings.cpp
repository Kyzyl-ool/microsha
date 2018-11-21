//
//  working_with_strings.cpp
//  microsha
//
//  Created by Кежик Кызыл-оол on 23.09.2018.
//  Copyright © 2018 Кежик Кызыл-оол. All rights reserved.
//

#include "working_with_strings.hpp"
#include <iostream>

int amount_of_symbol_in_string(const char* string, const char symbol)
{
    int result = 0;
    int i = 0;
    char x = string[i];
    while(x)
    {
        if (x == symbol)
            result++;
        x = string[++i];
    }
    
    return result;
}

char* last_symbol_in_string(const char* string, const char symbol)
{
    int result = 0;
    int i = 0;
    char x = string[i];
    while(x)
    {
        if (x == symbol)
            result = i;
        x = string[++i];
    }
    
    return (char* )(string+result+1);
}

int amount_of_symbol_in_string(std::string string, const char symbol)
{
    int result = 0;
    unsigned long len = string.size();
    for (int i = 0; i < len; i++) {
        if (string[i] == symbol)
            result++;
    }
    
    return result;
}

std::vector<std::string> split_string_by_separator(std::string str, char sep)
{
    std::vector<std::string> result;
    
    char* s = new char[str.size() + 1];
    strcpy(s, str.c_str());
    std::string tok = " ";
    tok[0] = sep;
    char* pch = strtok(s, tok.c_str());
    
    while (pch != NULL)
    {
        result.push_back(pch);
        pch = strtok(NULL, tok.c_str());
    }
    
    return result;
}


std::vector<std::string> get_arguments(std::string str)
{
    std::vector<std::string> result = split_string_by_separator(str, ' ');
    
    if (result.size() > 1) {
        result.erase(result.begin());
    }
    else
    {
        std::vector<std::string> empty;
        return empty;
    }
    
    return result;
}

std::string get_command_name(std::string str)
{
    std::vector<std::string> result = split_string_by_separator(str, ' ');
    
    if (result.size() > 0)
        return result[0];
    else
        return "";
}

std::string strip_first_word(std::string str)
{
    std::string null = "";
    return null;
}

std::string glue_strings_by(std::vector<std::string> strings, char sep)
{
    std::string result;

    for (int i = 0; i < strings.size(); i++) {
        result += strings[i]+sep;
    }
    result[result.size()-1] = '\0';

    return result;
}