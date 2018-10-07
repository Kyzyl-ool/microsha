//
//  microsha.hpp
//  microsha
//
//  Created by Кежик Кызыл-оол on 24.09.2018.
//  Copyright © 2018 Кежик Кызыл-оол. All rights reserved.
//

#ifndef microsha_hpp
#define microsha_hpp

#include <stdio.h>
#include <string>

class Program
{
protected:
    std::string name;
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
};

bool am_i_super(const char* argument);
std::string read_stdin();

#endif /* microsha_hpp */
