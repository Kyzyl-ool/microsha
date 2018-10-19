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

class microsha
{
private:
    std::string path = "";
    std::string IO_buffer = "";
    bool superuser = false;
    bool working = true;
public:
    microsha(std::string path);
    
    void run(void* args = NULL, size_t size = 0);
    void print_invitation();
    void print_version();
    void read_stdin();
    
    template <typename T>
    void print(T str);
};

#endif /* microsha_hpp */
