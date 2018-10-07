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
    
    printf("%s", last_symbol_in_string(argv[0], '/'));
    
    if (argc > 1 && am_i_super(argv[1]))
        printf("!");
    else
        printf(">");
    
    std::string test;
    getline(std::cin, test);
    
    
    //    std::cout << test << std::endl;
    
    std::vector<std::string> res = split_string_by_separator(test, ' ');
    
    for (int i = 0; i < res.size(); i++) {
        std::cout << res[i] << std::endl;
    }
    
    return 0;
}
