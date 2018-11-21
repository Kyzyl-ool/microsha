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

        conveyor(0, 1, IO_buffer);
        if (errno)
        {
            perror("microsha");
            errno = 0;
        }
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
    printf("%s\n", get_current_path().c_str());
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
//    printf("fdi: %d, fdo: %d\n", fdi, fdo);
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
            execute_external_program(fdi, fdo, command);
            break;
        }
    }

}

void microsha::execute_external_program(STANDARD_IO_ARGS, std::string command)
{
    pid_t pid = fork();
    if (pid == 0) {
        if (fdi != 0) dup2(fdi, 0);
        if (fdo != 1) dup2(fdo, 1);
        std::string command_name = get_command_name(command);
        std::vector<std::string> args = get_arguments(command);
        if (args.size() > 0) {
            char** arguments = (char **) calloc(args.size() + 2, sizeof(char *));
            arguments[0] = (char* )calloc(get_current_path().size(), sizeof(char));
            strcpy(arguments[0], get_current_path().c_str());
            arguments[0][get_current_path().size()] = '\0';

            for (int i = 1; i < args.size()+1; i++) {
                arguments[i] = (char *) calloc(args[i-1].size()+1, sizeof(char));
                strcpy(arguments[i], args[i-1].c_str());
                arguments[i][args[i-1].size()] = '\0';
            }
            arguments[args.size()+1] = NULL;

            execvp(command_name.c_str(), arguments);
        } else {
            char** null = (char** )calloc(1, sizeof(char*));
            null[0] = (char* )calloc(1, sizeof(char));
            execvp(command_name.c_str(), null);
        }
        perror(command_name.c_str());
        exit(0);
    }
    else {
        int status;
        wait(&status);

        if (fdi != 0) close(fdi);
        if (fdo != 1) close(fdo);
    }
}

void microsha::conveyor(STANDARD_IO_ARGS, std::string command)
{
    std::vector<std::string> conv = split_string_by_separator(command, '|');

    if (conv.size() == 2) {
        int* fd = new int[2];
        pipe(fd);

        execute(fdi, fd[1], conv[0]);
        execute(fd[0], fdo, conv[1]);
    }
    else if (conv.size() > 2) {
        int fd[conv.size()-1][2];
        for (int i = 0; i < conv.size() - 1; i++) {
            pipe(fd[i]);
        }

        execute(fdi, fd[0][1], conv[0]);
        for (int i = 1; i < conv.size()-1; i++) {
            execute(fd[i-1][0], fd[i][1], conv[i]);
        }
        execute(fd[conv.size()-2][0], fdo, conv[conv.size()-1]);

    }
    else {
        execute(fdi, fdo, command);
    }


}