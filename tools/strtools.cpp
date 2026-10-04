#include "strtools.hpp"

std::string toupper(const std::string& x){
    std::string res;
    for (int i = 0; i < x.size(); ++i){
            res += std::toupper(x[i]);
    }
    return res;
}