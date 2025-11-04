#include <iostream>

#include "resp_parser.hpp"

ParseResult RespParser::parse_ping_() {
    return ParseResult {"ping", {}};
}

ParseResult RespParser::parse_echo_() {
    int n;
    std::string line, payload;
    
    line = RespUtil::readline(iss_);

    // TODO: ECHO without payload has weird behaviour
    //  - Error when ECHO has not been executed before
    //  - Returns payload of prev ECHO command
    if (line[0] != '$')
        return ParseResult {"", {}};

    n = std::stoi(line.substr(1));
    payload = RespUtil::readline(iss_);

    if (payload.size() != n)
        return ParseResult {"", {}};

    return ParseResult {"echo", {payload}};
}

ParseResult RespParser::parse_set_() {
    int n;
    std::string line, key, value;
    
    // TODO: SET with less than 2 args cause error
    // Key extraction and validation
    line = RespUtil::readline(iss_);


    if (line[0] != '$')
        return ParseResult {"", {}};
    
    n = std::stoi(line.substr(1));
    key = RespUtil::readline(iss_);
    
    if (key.size() != n)
        return ParseResult {"", {}};

    // Value extraction and validation
    line = RespUtil::readline(iss_);

    if (line[0] != '$')
        return ParseResult {"", {}};
    
    n = std::stoi(line.substr(1));
    value = RespUtil::readline(iss_);

    if (value.size() != n)
        return ParseResult {"", {}};

    return ParseResult {"set", {key, value}};
}

ParseResult RespParser::parse_get_() {
    int n;
    std::string line, key;

    // Key extraction and validation
    line = RespUtil::readline(iss_);

    if (line[0] != '$')
        return ParseResult {"", {}};
    
    n = std::stoi(line.substr(1));
    key = RespUtil::readline(iss_);
    
    if (key.size() != n)
        return ParseResult {"", {}};
    
    return ParseResult {"get", {key}};
}

ParseResult RespParser::parse(char* buff, int len) {

    iss_ = std::istringstream(buff);
    buff_len_ = len;

    std::string line = RespUtil::readline(iss_);

    if (line[0] != '*') 
        return ParseResult {"", {}};

    // TODO: Below might break if it's not a number
    n_args_ = std::stoi(line.substr(1));

    line = RespUtil::readline(iss_);

    if (line[0] != '$')
        return ParseResult {"", {}};

    int cmd_len = std::stoi(line.substr(1));
    std::string cmd_val = RespUtil::readline(iss_);

    if (cmd_val.size() != cmd_len) 
        return ParseResult {"", {}};

    cmd_val = RespUtil::to_lower(cmd_val);

    if (fn_map_.count(cmd_val))
        return fn_map_[cmd_val]();

    return ParseResult {"", {}};

}