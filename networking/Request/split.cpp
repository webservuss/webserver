#include "split.hpp"


std::vector<std::string> utils::split_http(const std::string &str, const std::string& split_c){

    size_t start = 0;
    size_t end   = 0;
    std::string res;
    std::vector<std::string> array;

    std::cout << "str**** " << str << std::endl;
    std::cout << "strsize " << str.size() << std::endl;
    std::cout << "split_c " << split_c << "c" << std::endl;
    //end = str.size();
    // while(str[start] != end){
    //     while(str[res] != " ")
    // }

    /* FIRST PART  */
    end = str.find(" ");
    std::cout << "print the lengt of end ----" <<end << std::endl;
    /* npos is a static member constant value with the greatest 
    possible value for an element of type size_t */
    // while(end != str.npos)
    // {
        
    //     /* find the start position by finding the split char 
    //     find_first_not_of searches seraches the string for the first character that does 
    //     not match any of the characters specified in the argument */
    //     std::cout << "print the lengt of end" <<end << std::endl;
        //start = str.find_first_not_of(split_c, end);
        //end = str.find_first_not_of(split_c ,start);
        std::cout << "print the lengt of start" <<start << std::endl;
        std::cout << "print the lengt of end" <<end << std::endl;
    //     /* so basicly checking if not empty */
    //     std::cout << "end - start" <<end - start << std::endl;
    //     std::cout << "strsize " << str.size() << std::endl;
    //     //res = str.substr(start, end - start);
    //     std::cout << "res"<< res << std::endl;
        // make a loop to fill the vector 
        if(end != std::string::npos || start != std::string::npos)
        {
            std::cout << "in pushback"  << std::endl;
            res = str.substr(start, end - start);
            std::cout << "res" << res << std::endl;
            array.push_back(str.substr(start, end - start));
        }
    //}
    std::cout << " array size "<< array.size()<< std::endl;
    for(int i = 0; i <array.size(); i++)
    {
        std::cout << "after split" << array[i] << std::endl;
    }
    return(array);
}
