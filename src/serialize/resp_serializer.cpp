#include "resp_serializer.hpp"

std::string RespSerializer::simple_string_(const Response& response) {
    // TODO: Below might break if repsonse doesn't adhere to the contract
    return "+" + response.args.at(0) + "\r\n";
}

std::string RespSerializer::bulk_string_(const Response& response) {
    if (response.args.empty())
        return "$-1\r\n";

    std::string output = "";
    for (const std::string& arg : response.args) {
        output += "$" + std::to_string(arg.size()) + "\r\n" + arg + "\r\n";
    }
    return output;
}

std::string RespSerializer::simple_error_(const Response& response) {
    // TODO: Below might break if repsonse doesn't adhere to the contract
    return "-" + response.args.at(0) + "\r\n";
}

std::string RespSerializer::serialize(const Response& response) {
    switch (response.type) {
        case Type::SimpleString:
            return simple_string_(response);
            break;
        case Type::BulkString:
            return bulk_string_(response);
            break;
        case Type::SimpleError:
            return simple_error_(response);
            break;
    }
}