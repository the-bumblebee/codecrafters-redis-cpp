#include "resp_parser.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

std::string RespUtil::parse(char* buff, int len) {
    std::string request(buff, len);
    std::string request_lower = request;
    std::transform(request_lower.begin(), request_lower.end(), request_lower.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    
    if (request_lower.find("ping") != std::string::npos) {
        return "+PONG\r\n";
    } else if (request_lower.find("echo") != std::string::npos) {
        return request.substr(14);
    }

    return "-ERR\r\n";
}