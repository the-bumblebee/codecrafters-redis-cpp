#include "resp.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

std::string RespParser::parse_ping(std::istringstream& iss) {
    std::string response_msg = "PONG";
    return RespSerializer::simple_string(response_msg);
}

std::string RespParser::parse_echo(std::istringstream& iss) {
    std::string error_msg;
    std::string line = RespUtil::readline(iss);

    if (line[0] != '$') {
        error_msg = "Invalid format!";
        return RespSerializer::simple_error(error_msg);
    }

    int n = std::stoi(line.substr(1));
    std::string message = RespUtil::readline(iss);

    if (message.size() != n) {
        error_msg = "Invalid format!";
        return RespSerializer::simple_error(error_msg);
    }

    return RespSerializer::bulk_string(message);
}

std::string RespParser::parse(char* buff, int len) {

    std::string error_msg;

    std::istringstream iss(buff);
    std::string line = RespUtil::readline(iss);

    if (line[0] != '*') {
        std::string error_msg = "Invalid format!";
        return RespSerializer::simple_error(error_msg);
    }

    // int n = std::stoi(line.substr(1));

    line = RespUtil::readline(iss);

    if (line[0] != '$') {
        error_msg = "Invalid format!";
        return RespSerializer::simple_error(error_msg);
    }

    int command_length = std::stoi(line.substr(1));
    std::string command = RespUtil::readline(iss);

    if (command.size() != command_length) {
        error_msg = "Invalid format!";
        return RespSerializer::simple_error(error_msg);
    }

    command = RespUtil::to_lower(command);

    if (parser_map.count(command)) {
        return parser_map[command](iss);
    }

    error_msg = "Unkown command!";
    return RespSerializer::simple_error(error_msg);
}

std::string RespUtil::readline(std::istringstream& iss) {
    std::string line;
    std::getline(iss, line);
    if (line.back() == '\r') line.pop_back();
    return line;
}

std::string RespUtil::to_lower(std::string& value) {
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return result;
}

std::string RespSerializer::simple_string(std::string& value) {
    return "+" + value + "\r\n";
}

std::string RespSerializer::bulk_string(std::string& value) {
    return "$" + std::to_string(value.size()) + "\r\n" + value + "\r\n";
}

std::string RespSerializer::simple_error(std::string& value) {
    return "-ERR " + value + "\r\n";
}