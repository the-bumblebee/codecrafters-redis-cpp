#ifndef RESP_UTIL_HPP
#define RESP_UTIL_HPP

#include <string>

class RespUtil {
private:

public:
    static std::string parse(char* buff, int len);
};

#endif