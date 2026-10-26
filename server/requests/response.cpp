#include "response.hpp"


std::string Response::to_string() {
    std::map<std::string, std::string> result_dict;
    std::string result_str = "";
    result_dict["status"] = std::to_string(this->status);
    
    std::map<std::string, std::string>::iterator iter = headers.begin();
    std::string headers_str = "{'";
    headers_str += iter->first + "':'" + iter->second + "'";
    for(++iter; iter != headers.end(); ++iter) {
        headers_str += ",'" + iter->first + "':'" + iter->second + "'";
    }
    headers_str += "}\n";
    
    result_dict["header"] = headers_str;
    result_dict["body"] = body;

    result_str = "{'status': " + result_dict["status"] + 
                 ",'headers': " + result_dict["headers"] + 
                 ",'body': " + result_dict["body"];
    return result_str;
}