#ifndef RESP_UTIL_HPP
#define RESP_UTIL_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

enum Type {
    SimpleString,
    SimpleError,
    BulkString
};

struct ParseResult {
    std::string command;
    std::vector<std::string> args;
};

struct Response {
    Type type;
    std::vector<std::string> args;
};

class RespUtil {
public:
    static std::string readline(std::istringstream& iss) {
        std::string line;
        std::getline(iss, line);
        if (line.back() == '\r') line.pop_back();
        return line;
    };

    static std::string to_lower(std::string& value) {
        std::string result = value;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); }
        );
        return result;
    };
};

#endif