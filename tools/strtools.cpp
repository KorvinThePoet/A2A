#include "strtools.hpp"

std::string toupper(const std::string& x){
    std::string res;
    for (int i = 0; i < x.size(); ++i){
            res += std::toupper(x[i]);
    }
    return res;
}

size_t string_to_size_t(const std::string& x){
    size_t res = 0;
    for (int i = 0; i < x.size(); ++i){
        if(x[i] < '0' || x[i] > '9') {
            return 0;
        }
        res = res*10 + (x[i] - '0');
    }
    return res;
}