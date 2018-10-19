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
    
}

void microsha::run(void *args, size_t size)
{
    print_invitation();
    read_stdin();
    while (IO_buffer != "exit") {
        std::vector<std::string> split = split_string_by_separator(IO_buffer, ' ');
        print(split.size());
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
    getline(std::cin, IO_buffer);
}

template <typename T>
void microsha::print(T value)
{
    std::cout << value << std::endl;
}
