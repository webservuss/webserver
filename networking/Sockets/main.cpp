#include "listeningSocket.hpp"

int main ()
{
    std::cout << "starting ...." << std::endl;
    std::cout << "binding Socket ...." << std::endl;
    HDE::bindingSocket bs = HDE::bindingSocket(AF_INET, SOCK_STREAM, 0, 81, INADDR_ANY);
     std::cout << "ListeningSocket ...." << std::endl;
    HDE::listentingSocket ls = HDE::listentingSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
    std::cout << "succes ...." << std::endl;
}