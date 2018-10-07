//
//  microsha.cpp
//  microsha
//
//  Created by Кежик Кызыл-оол on 24.09.2018.
//  Copyright © 2018 Кежик Кызыл-оол. All rights reserved.
//

#include "microsha.hpp"
#include "working_with_strings.hpp"
#include <string>
#include <iostream>

bool am_i_super(const char* argument)
{
    return (!strcmp(argument, "-superman")) ? true : false;
}

std::string read_stdin()
{
    std::string result;
    std::cin >> result;
    
    return result;
}

microsha::microsha(std::string path):
path(path)
{

}

void microsha::run(void *args, size_t size)
{
    printf("%s", last_symbol_in_string(path.c_str(), '/'));
    if(superuser)
        printf("!");
    else
        printf(">");
}
