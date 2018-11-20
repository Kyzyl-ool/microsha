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
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

const int MAXDIR = 2048;

microsha::microsha(std::string path):
IO_buffer(""),
superuser(false),
working(true)
{
    hasbtable["cd"] = 1;
    hasbtable["pwd"] = 2;
    hasbtable[""] = 3;
    hasbtable["time"] = 4;
    hasbtable["exit"] = 5;

    home = getenv("HOME");
}

void microsha::run(void *args, size_t size)
{
    while (working) {
        print_invitation();
        read_stdin();

        int new_input = dup(0);
        int new_output = dup(1);
//        execute(new_input, new_output, IO_buffer);
        conveyor(new_input, new_output, IO_buffer);
        if (errno)
        {
            perror("microsha");
            errno = 0;
        }

        close(new_input);
        close(new_output);
//        printf("[%d]", IO_buffer.c_str()[0]);
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
        std::string::size_type index;
        while((index=args[0].find("~"))!=std::string::npos)
        args[0].replace(index, 1, home);
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
    print(get_current_path().c_str());
}

void microsha::time(STANDARD_IO_ARGS, std::string command)
{
    command.erase(0, 5);
    clock_t start = clock();
    execute(fdi, fdo, command);
    clock_t end = clock();
    
    dprintf(2, "%.3lf secs\n",
             (double)(end - start) / CLOCKS_PER_SEC);
}

void microsha::execute(STANDARD_IO_ARGS, std::string command)
{
    printf("fdi: %d, fdo: %d\n", fdi, fdo);
    dup2(fdi, 0); dup2(fdo, 1);
    std::vector<std::string> arguments = get_arguments(command);
    switch (hasbtable[get_command_name(command)]) {
        case 1:
            cd(fdi, fdo, arguments);
            break;
        case 2:
            pwd(fdi, fdo);
            break;
        case 3:
            break;
        case 4:
            time(fdi, fdo, command);
            break;
        case 5:
            working = false;
            break;
        default: {
            pid_t pid = fork();
            if (pid == 0) {
                execute_external_program(fdi, fdo, command);
                perror("microsha");
                exit(0);
            }
            else
            {
                int status;
                wait(&status);
            }
            break;
        }
    }
}

void microsha::execute_external_program(STANDARD_IO_ARGS, std::string command)
{
    std::string command_name = get_command_name(command);
    std::vector<std::string> args = get_arguments(command);
    char **arguments = nullptr;
    if (args.size() > 0) {
        arguments = (char **) calloc(args.size(), sizeof(char *));
        for (int i = 0; i < args.size(); i++) {
            arguments[i] = (char *) calloc(args[i].size(), sizeof(char));
            strcpy(arguments[i], args[i].c_str());
            arguments[i][args[i].size()] = '\0';
        }
        execvp(command_name.c_str(), arguments);
    }
    else {
        execvp(command_name.c_str(), NULL);
    }
    perror(command_name.c_str());
    exit(0);
}

void microsha::conveyor(STANDARD_IO_ARGS, std::string command)
{
    std::vector<std::string> conv = split_string_by_separator(command, '|');

    if (conv.size() == 2) {
        int* fd = new int[2];
        pipe(fd);

        execute(fdi, fd[1], conv[0]);
        execute(fd[0], fdo, conv[1]);

        close(fd[0]);
        close(fd[1]);
    }
    else if (conv.size() > 2) {
        for (int i = 0; i < conv.size(); i++) {
            printf("%d: %s\n", i, conv[i].c_str());
        }
    }
    else {
        execute(fdi, fdo, command);
    }


}