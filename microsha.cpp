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
#include <dirent.h>
#include <glob.h>

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

        std::vector <std::string> args2;
        for (int i = 0; i < args.size(); i++) {
            std::vector<std::string> tmp;
            if (args[i].find('*') != std::string::npos || args[i].find('?') != std::string::npos) {
                tmp = parseOneDepth(args[i]);






                args2.insert(args2.end(), tmp.begin(), tmp.end());
            }
            else {
                args2.push_back(args[i]);
            }
        }

        args = args2;

//        printf("cmd: %s, args: %s\n", get_command_name(command).c_str(), glue_strings_by(args, ';').c_str());

        if (args.size() > 0) {
            char** arguments = (char **) calloc(args.size() + 2, sizeof(char *));
            arguments[0] = (char* )calloc(get_current_path().size()+1, sizeof(char));
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
            char** null = (char** )calloc(2, sizeof(char*));
            null[0] = (char* )calloc(get_current_path().size()+1, sizeof(char));
            strcpy(null[0], get_current_path().c_str());
            null[1] = (char* )calloc(1, sizeof(char));
            null[1] = NULL;
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



std::vector<std::string> microsha::parseOneDepth(std::string expression) {
    glob_t glob_result;

    int glob_return_value = glob(expression.c_str(), GLOB_TILDE, NULL, &glob_result);
    if (glob_return_value != 0) {
        globfree(&glob_result);
        perror("glob");

    }

    std::vector <std::string> pathnames;
    for (int i = 0; i < glob_result.gl_pathc; i++) {
        pathnames.push_back(std::string(glob_result.gl_pathv[i]));
    }

    globfree(&glob_result);

    return pathnames;

}

std::vector<std::string> microsha::find_matching_files(std::string pattern, std::string directory) {
    auto files = get_files_in_directory(directory);
    std::vector <std::string> matching_files;
    int result_size = 0;
    int words_count = 0;

    for (int i = 0; i < files.size(); i++) {
        if (files[i] != ".." && files[i] != "." && !fnmatch(pattern.c_str(), files[i].c_str(), FNM_PATHNAME)) {
            matching_files.push_back(files[i]);
            result_size += files[i].size();
            words_count++;
        }
    }

    return matching_files;
}

std::vector <std::string> microsha::get_files_in_directory(std::string dirname) {
    std::vector <std::string> result;
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(dirname.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            result.push_back(ent->d_name);
        }
    }
    else {
        perror("get_files_in_directory");
        return result;
    }

    return result;
}