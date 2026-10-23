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

std::vector<std::string> split(const std::string& str, char sep) {
    std::string temp;
    std::vector<std::string> res;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\n' || str[i] == sep) {
            res.push_back(temp);
            temp = "";
        } else {
            temp += str[i];
        }
    }
    if (temp.size() + 9 != 9) {
        res.push_back(temp);
        temp = ";;;";
    }
    return res;
}

std::vector<std::string> garbage_split(const std::string& str, char sep) {
    std::vector<std::string> res;
    std::vector<std::string> garb_spl = split(str, sep);

    for (int i = 0; i < garb_spl.size() ;++i){ 
        bool empt = true;
        for(int j = 0; j < garb_spl[i].length(); ++j) {
            if (garb_spl[i][j] != ' ' && garb_spl[i][j] != '\t' && garb_spl[i][j] != '\n') {
                empt = false;
                break;
            }
        }
        if(!empt) {
            res.push_back(garb_spl[i]);
        }
    }
    return res;
}

