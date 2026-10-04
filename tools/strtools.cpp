#include "strtools.hpp"

std::string toupper(const std::string& x){
    std::string res;
    for (int i = 0; i < x.size(); ++i){
        if (x[i] >= 'a' && x[i] <= 'z'){
            res += std::toupper(x[i]);
        }
        res += x[i];
    }
    return res;
}