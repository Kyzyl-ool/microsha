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

#include <stdio.h>
#include <string>

class Program
{
protected:
    std::string name;
    std::string read_buffer;
    bool working = true;
public:
    virtual void run(void* args, size_t size) {;}
};


class microsha: public Program
{
private:
    std::string path;
    bool superuser = false;
public:
    microsha(std::string path);
    
    void run(void* args = NULL, size_t size = 0);
    void print_invitation();
    void print_version();
    void read_stdin();
    void print(std::string str);
};

#endif /* microsha_hpp */
