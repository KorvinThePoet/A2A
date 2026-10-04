#include <iostream>
#include "request.hpp"
#include "strtools.hpp"

Request::Request(const std::string& str) : method(0), route(""), host(""), content_type(""),
                                            content_length(0), bad_request(false){
    std::string met, rou, hst, tp, len;
    int i = 0;
    for (; str[i] != ' '; ++i){
        met += str[i];
    }
    if (toupper(met) == "POST"){
        method = 0;
    } else if (toupper(met) == "GET"){
        method = 1;
    } else if (toupper(met) == "PUT"){
        method = 2;
    } else if (toupper(met) == "DELETE"){
        method = 3;
    } else {
        bad_request = true;
        return;
    }
    ++i;
    for (; str[i] != '\n' && i < str.size(); ++i){
        rou += str[i];
    }
    ++i;
    size_t pos = toupper(str).find(toupper("Host: "), i);
    if (i != pos){
        bad_request = true;
        return;
    }
    i += 6;
    for (; str[i] != '\n' && i < str.size(); ++i){
        hst += str[i];
    }
    ++i;
    size_t pos_type = toupper(str).find(toupper("Content-type: "), i);
    if (i != pos_type){
        bad_request = true;
        return;
    }
    i += 14;
    for (; str[i] != '\n'; ++i){
        tp += str[i];
    }
    ++i;
    size_t pos_len = toupper(str).find(toupper("Content-length: "), i);
    if (i != pos_len){
        bad_request = true;
        return;
    }
    i += 16;
    for (; str[i] != '\n' && i < str.size(); ++i){
        len += str[i];
    }
    
}