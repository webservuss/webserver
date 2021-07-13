#ifndef split_hpp
#define split_hpp

#include <vector>
#include <string>
#include "../../http.hpp"
#include "../Servers/simple_server.hpp"
#include "../colors.hpp"
#include "makeMap.hpp"


namespace utils{

    std::vector<std::string>  split_http(const std::string &str, const std::string& split_c);
}

#endif