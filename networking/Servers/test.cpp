#include "parser_config.hpp"
#include "select_server.hpp"
#include <vector>

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
    // int ports[3] = {80, 50, 70};
    // HTTP::select_server t1(ports, 3);
    // HTTP::select_server t2;
    std::vector<int> ports;

    ports.push_back(4000);
    ports.push_back(5000);
    ports.push_back(7000);
    ports.push_back(8000);
    std::cout << "about to call ma stuff" << std::endl;
    HTTP::select_server t(ports) ;
}
