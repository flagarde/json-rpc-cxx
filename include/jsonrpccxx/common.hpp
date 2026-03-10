#pragma once
#include "nlohmann/json.hpp"
#include <exception>
#include <string>

namespace jsonrpccxx {
  typedef nlohmann::json json;

  static inline bool has_key(const json &v, const std::string &key) { return v.find(key) != v.end(); }
  static inline bool has_key_type(const json &v, const std::string &key, json::value_t type) { return has_key(v, key) && v.at(key).type() == type; }
  static inline bool valid_id(const json &request) {
    return has_key(request, "id") && (request["id"].is_number() || request["id"].is_string() || request["id"].is_null());
  }
  static inline bool valid_id_not_null(const json &request) { return has_key(request, "id") && (request["id"].is_number() || request["id"].is_string()); }

} // namespace jsonrpccxx
