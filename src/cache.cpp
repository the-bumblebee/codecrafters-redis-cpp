#include <iostream>
#include <string>
#include <unordered_map>

#include "cache.hpp"

void Cache::set(int client_fd, const std::string& key, const std::string& value) {
    // cache_[client_fd];
    // cache_.at(client_fd).insert(key, value);
    cache_[client_fd][key] = value;
}

std::optional<std::string> Cache::get(int client_fd, const std::string& key) {
    if (cache_.count(client_fd)) {
        std::unordered_map<std::string, std::string> map = cache_.at(client_fd);
        if (map.count(key)) return map.at(key);
    }
    return std::nullopt;
}

void Cache::remove_fd(int client_fd) {
    cache_.erase(client_fd);
}