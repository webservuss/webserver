#include "stdio.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>
#include <sstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <map>
#include <iostream>
#include <map>
#include <fstream>
#include "testserver.hpp"
#include "parser_config.hpp"
#include "parser_config.hpp"



int main()
{
    std::cout<< RED<< "MAIN" << RESET << std::endl;
    std::
    std::ifstream file("example.txt");
    file.open("example.txt", std::ifstream::in);
    if(file.is_open()){
        std::cout<< RED<< "###########CONFIG OPEN##########" << RESET << std::endl;
        HDE::parser_config t (file);
    }
    else
        std::cout << RED << " ERROR no configfile" << RESET << std::endl;
    file.close();

    //HDE::parser_config_open r();
    HDE::TestServer t;
}
