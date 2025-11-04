#ifndef CACHE_HPP
#define CACHE_HPP

#include <optional>
#include <string>
#include <unordered_map>

class Cache {
private:
    std::unordered_map<int, std::unordered_map<std::string, std::string>> cache_;

public:
    void set(int client_fd, const std::string& key, const std::string& value);
    std::optional<std::string> get(int client_fd, const std::string& key);
    void remove_fd(int client_fd);
};

#endif