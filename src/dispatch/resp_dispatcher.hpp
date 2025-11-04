#ifndef RESP_DISPATCHER_HPP
#define RESP_DISPATCHER_HPP

#include "../util/resp_util.hpp"
#include "../cache.hpp"

class RespDispatcher {
private:
    Cache& cache_;
    Response dispatch_ping_(const ParseResult& parse_result);
    Response dispatch_echo_(const ParseResult& parse_result);
    Response dispatch_set_(int client_fd, const ParseResult& parse_result);
    Response dispatch_get_(int client_fd, const ParseResult& parse_result);
public:
    RespDispatcher(Cache& cache) : cache_(cache) {}
    Response dispatch(int client_fd, const ParseResult& parse_result);
};

#endif