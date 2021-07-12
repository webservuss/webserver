#ifndef map_hpp
#define map_hpp

#include "../../http.hpp"
#include "../Servers/simple_server.hpp"
#include "../Colors.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <iterator>

#include <map>

namespace UTILS{

    template<typename I, typename S>
    class makeMap{
    private:
        std::map<I,S>_map;
    public:
        makeMap(const I &key, const S &value){
            _map[key] = value;
        }
        makeMap<I,S> &operator()(const I &key, const S &value ){
            _map[key] = value;
            return *this;
        }
        operator std::map<I,S>(){
            return _map;
        }
    };
}

#endif