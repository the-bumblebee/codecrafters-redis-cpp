#ifndef RESP_SERIALIZER_HPP
#define RESP_SERIALIZER_HPP

#include <string>

#include "../util/resp_util.hpp"

class RespSerializer {
private:
    std::string simple_string_(const Response& response);
    std::string simple_error_(const Response& response);
    std::string bulk_string_(const Response& response);

public:
    std::string serialize(const Response& response);
};

#endif