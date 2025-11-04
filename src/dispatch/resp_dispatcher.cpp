#include <iostream>
#include <optional>

#include "resp_dispatcher.hpp"

Response RespDispatcher::dispatch_ping_(const ParseResult& parse_result) {
    // if (!parse_result.args.empty()) {
    //     std::cerr << "[ERROR] ParseResult args is non-empty.";
    //     exit(1);
    // }
    
    return Response {Type::SimpleString, {"PONG"}};
}

Response RespDispatcher::dispatch_echo_(const ParseResult& parse_result) {
    return Response {Type::BulkString, parse_result.args};
}

Response RespDispatcher::dispatch_set_(int client_fd, const ParseResult& parse_result) {
    if (parse_result.args.size() <= 1)
        return Response {Type::SimpleError, {"ERR Wrong number of arguments for 'set' command"}};

    cache_.set(client_fd, parse_result.args.at(0), parse_result.args.at(1));
    return Response {Type::SimpleString, {"OK"}};
}

Response RespDispatcher::dispatch_get_(int client_fd, const ParseResult& parse_result) {
    if (parse_result.args.size() == 0)
        return Response {Type::SimpleError, {"ERR Wrong number of arguments for 'get' command"}};

    std::optional<std::string> data = cache_.get(client_fd, parse_result.args.at(0));

    if (data)
        return Response {Type::BulkString, {data.value()}};

    return Response {Type::BulkString, {}};
}

Response RespDispatcher::dispatch(int client_fd, const ParseResult& parse_result) {
    if (parse_result.command == "ping") return dispatch_ping_(parse_result);
    if (parse_result.command == "echo") return dispatch_echo_(parse_result);
    if (parse_result.command == "set") return dispatch_set_(client_fd, parse_result);
    if (parse_result.command == "get") return dispatch_get_(client_fd, parse_result);

    return Response {Type::SimpleError, {"ERR Incorrect format or command"}};
}