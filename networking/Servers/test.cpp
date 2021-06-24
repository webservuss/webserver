#include "stdio.h"
#include "testserver.hpp"


int main()
{
    // HDE::TestServer t;
    ifstream    configfileStream;
    std::string configfilePath;

    configfilePath = "../configs/example.conf"
    configfileStream.open(configfilePath);
    if (!configfileStream)
    {
        std::cout<< "unable to open " << configfilePath << std::endl;
        return (-1);
    }
    HDE::configServer configServ(configfileStream);
    return (0);
}
