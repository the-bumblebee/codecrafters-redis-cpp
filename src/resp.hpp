#ifndef RESP_HPP
#define RESP_HPP

#include <string>
#include <sstream>
#include <unordered_map>

class RespParser {
private:
    static std::string parse_ping(std::istringstream& iss);
    static std::string parse_echo(std::istringstream& iss);
    inline static std::unordered_map<std::string, std::string(*)(std::istringstream&)> parser_map {
        {"ping", &RespParser::parse_ping},
        {"echo", &RespParser::parse_echo}
    };
public:
    static std::string parse(char* buff, int len);
};

class RespUtil {
public:
    static std::string readline(std::istringstream& iss);
    static std::string to_lower(std::string& value);
};

class RespSerializer {
public:
    static std::string simple_string(std::string& value);
    static std::string simple_error(std::string& value);
    static std::string bulk_string(std::string& value);
};

#endif