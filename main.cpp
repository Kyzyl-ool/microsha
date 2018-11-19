//
//  main.cpp
//  microsha
//
//  Created by Кежик Кызыл-оол on 23.09.2018.
//  Copyright © 2018 Кежик Кызыл-оол. All rights reserved.
//

#include <iostream>
#include <string>
#include "working_with_strings.hpp"
#include "microsha.hpp"


int main(int argc, const char * argv[]) {
    std::string path = argv[0];
    microsha mainMicrosha(path);
    mainMicrosha.run();
    
    return 0;
}
