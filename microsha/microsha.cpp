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

microsha::microsha(std::string path):
path(path)
{
    name = "microsha";
}

void microsha::run(void *args, size_t size)
{
    print_invitation();
    read_stdin();
    while (read_buffer != "exit") {
        print(read_buffer);
        print_invitation();
        read_stdin();
    }
}

void microsha::print_invitation()
{
    printf("%s", last_symbol_in_string(path.c_str(), '/'));
    printf("%c", '>' - 29*superuser);
}

void microsha::print_version()
{
    printf(VERSION);
    printf("\n");
}

void microsha::read_stdin()
{
    getline(std::cin, read_buffer);
}

void microsha::print(std::string str)
{
    std::cout << str << std::endl;
}
