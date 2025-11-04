#ifndef RESP_PARSER_HPP
#define RESP_PARSER_HPP

#include <functional>
#include <sstream>
#include <unordered_map>

#include "../util/resp_util.hpp"

class RespParser {
private:
    std::istringstream iss_;
    int buff_len_;
    int n_args_;
    std::unordered_map<std::string, std::function<ParseResult()>> fn_map_;

    ParseResult parse_ping_();
    ParseResult parse_echo_();
    ParseResult parse_set_();
    ParseResult parse_get_();

    auto make_fn_map_() {
        return std::unordered_map<std::string, std::function<ParseResult()>> {
            {"ping", [this]{ return parse_ping_(); }},
            {"echo", [this]{ return parse_echo_(); }},
            {"set", [this]{ return parse_set_(); }},
            {"get", [this]{ return parse_get_(); }}
        };
    }

public:
    RespParser() : fn_map_(make_fn_map_()) {};
    ParseResult parse(char* buff, int len);
};

#endif