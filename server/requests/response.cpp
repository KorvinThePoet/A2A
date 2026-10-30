#include "response.hpp"


std::string Response::to_string() {
    std::string result_str = "{'status': " + std::to_string(status);

    if (!headers.empty()) {
        auto iter = headers.begin();
        std::string headers_str = "{'" + iter->first + "':'" + iter->second + "'";
        for (++iter; iter != headers.end(); ++iter) {
            headers_str += ",'" + iter->first + "':'" + iter->second + "'";
        }
        headers_str += "}";
        result_str += ",'headers': " + headers_str;
    }
    if(!body.empty())
        result_str += ",'body': " + body + "}";
    else
        result_str += "}";
    return result_str;
}
