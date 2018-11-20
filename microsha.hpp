//
//  microsha.hpp
//  microsha
//
//  Created by Кежик Кызыл-оол on 24.09.2018.
//  Copyright © 2018 Кежик Кызыл-оол. All rights reserved.
//

#ifndef microsha_hpp
#define microsha_hpp

#define VERSION "1.0"
#define STANDARD_IO_ARGS int fdi, int fdo

const char METASYMBOL_GREAT = '>';
const char METASYMBOL_LESS  = '<';
const char METASYMBOL_DASH  = '|';
const char METASYMBOL_STAR  = '*';
const char METASYMBOL_QUESTION = '?';

#include <stdio.h>
#include <string>
#include <map>

class microsha
{
private:
    std::string IO_buffer;
    std::string home;
    bool superuser;
    std::map <std::string, int> hasbtable;
    
public:
    microsha(std::string path);
    
    void run(void* args = NULL, size_t size = 0);
    void print_invitation();
    void print_version();
    void read_stdin();
    void add_slash_at_end(std::string str);
    void execute(STANDARD_IO_ARGS, std::string command);
    void execute_external_program(STANDARD_IO_ARGS, std::string command);
    void conveyor(STANDARD_IO_ARGS, std::string command);
    
    template <typename T>
    void print(T str);
    
    
    void cd(STANDARD_IO_ARGS, std::vector<std::string> args);
    void pwd(STANDARD_IO_ARGS);
    void time(STANDARD_IO_ARGS, std::string command);
    
    
    std::string get_current_path();
};

#endif /* microsha_hpp */
