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
#include <unistd.h>
#include <fnmatch.h>
#include <string>
#include "stdio.h"
#include <time.h>

const int MAXDIR = 2048;

microsha::microsha(std::string path)
{
    hasbtable["cd"] = 1;
}

void microsha::run(void *args, size_t size)
{
    print("microsha by Kyzyl-ool.\nEnter 'exit' to finish the program.\n");
    print_invitation();
    read_stdin();
    while (IO_buffer != "exit") {
        std::vector<std::string> arguments = split_string_by_separator(IO_buffer, ' ');
        std::string command = arguments[0];
        arguments.erase(arguments.begin());
        

        //WORK SECTION
        execute(hasbtable[command], 0, 1, arguments);
        if (errno != 0) {
            print(strerror(errno));
        }
        errno = 0;

        print_invitation();
        read_stdin();
    }
    
}

void microsha::print_invitation()
{
    printf("%s", last_symbol_in_string(get_current_path().c_str(), '/'));
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

bool isdirectory(std::string dir)
{
    //NEED TO IMPLEMENT
    return true;
}

void microsha::add_slash_at_end(std::string str)
{
    if (str.back() != '/')
        str.push_back('/');
}

void microsha::cd(STANDARD_IO_ARGS, std::vector<std::string> args)
{
    if (args.size() == 0)
    {
        chdir(getenv("HOME"));
    }
    else if (args.size() == 1)
    {
        add_slash_at_end(args[0]);
        chdir(args[0].c_str());
    }
}

std::string microsha::get_current_path()
{
    char dir[MAXDIR];
    getcwd(dir, MAXDIR);
    return dir;
}

void microsha::pwd(STANDARD_IO_ARGS)
{
    dprintf(fdo, "%s\n", get_current_path().c_str());
}

void microsha::time(STANDARD_IO_ARGS, std::vector<std::string> command)
{
    
//    clock_t start = clock();

}

void microsha::execute(int program_number, STANDARD_IO_ARGS, std::vector<std::string> arguments)
{
    switch (program_number) {
        case 1:
            cd(fdi, fdo, arguments);
            break;
        default:
            print("Unknown command.\n");
            break;
    }
}
