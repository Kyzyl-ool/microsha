//
//  working_with_strings.hpp
//  microsha
//
//  Created by Кежик Кызыл-оол on 23.09.2018.
//  Copyright © 2018 Кежик Кызыл-оол. All rights reserved.
//

#ifndef working_with_strings_hpp
#define working_with_strings_hpp

#include <iostream>
#include <vector>
#include <string>

int amount_of_symbol_in_string(const char* string, const char symbol);
int amount_of_symbol_in_string(std::string string, const char symbol);

char* last_symbol_in_string(const char* string, const char symbol);
std::vector<std::string> split_string_by_separator(std::string str, char sep);

std::vector<std::string> get_arguments(std::string str);
std::string get_command_name(std::string str);
std::string strip_first_word(std::string str);

#endif /* working_with_strings_hpp */
