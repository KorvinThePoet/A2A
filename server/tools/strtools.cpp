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


std::vector<std::string> devide_in_two(const std::string& str, int start, char sep) {

    std::string prefix;
    std::string postfix;
    int i = start;

    for(;i < str.length(); ++i) {
        if(str[i] == sep) {
            ++i;
            break;
        }
        prefix += str[i];
    }
    for(; i <str.length(); ++i) {
        postfix += str[i];
    }

    return {prefix, postfix};
}

bool starts_with(const std::string& dep, const std::string& arr) {
    int i = 0;
    for(; i < dep.length() && i < arr.length(); ++i) {
        if(dep[i] != arr[i]) {
            return false;
        }
    }

    if (i == dep.length() && i != arr.length()) {
        return false;
    }
    return true;
}

std::map<std::string, std::string> to_map(const std::string& str) {
    // TODO дырявое ведро хихи
    // ALLOWED ONLY single quotion mark ("'")

    std::map<std::string, std::string> result;
    std::vector<std::string> pairs = split(str, ',');

    for(std::string pair : pairs) {
        std::string curr_key;
        std::string curr_val;
        int i = 0;
        bool seen = false;
        for(; i < pair.length(); ++i) {
            if(pair[i] == '\'' && seen) {
                ++i;
                break;
            }
            if(seen) {
                curr_key += pair[i];
            }
            if(pair[i] == '\'') {
                seen = true;
                continue;
            }
        }
        
        seen = false;
        bool norm = false;
        for(; i < pair.length(); ++i) {
            if(pair[i] == ':') {
                if(!norm)
                    norm = true;
                else{
                    norm = false;
                    break;
                }
            }
            if(seen && pair[i] == '\'') {
                break;
            }
            if(seen) {
                curr_val += pair[i];
            }
            if(pair[i] == '\'') {
                seen = true;
            }
        }

        if(!norm) {
            std::cout << "ERROR WHILE PARSING";
            throw std::runtime_error("PARSE ERROR");
        }

        result[curr_key] = curr_val;
    }
    return result;
}
