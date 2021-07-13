#include "parser_config.hpp"
#include "select_server.hpp"

int main()
{
    std::cout<< RED<< "MAIN" << RESET << std::endl;
    std::ifstream file;
    // std::ifstream file("example.txt");
    std::string path;
    path = "configs/example.txt";
    // file.open(path , std::ifstream::in);
    file.open(path);
    if(file.is_open()){
        std::cout<< RED<< "###########CONFIG OPEN##########" << RESET << std::endl;
        HDE::parser_config t (file);
    }
    else
        std::cout << RED << " ERROR no configfile" << RESET << std::endl;
    file.close();

    //HDE::parser_config_open r();
    HTTP::select_server t;
}
