#pragma once
#include "nlohmann/json.hpp"

namespace jsonrpccxx {
  typedef nlohmann::json json;

  static inline bool valid_id(const json &request) {
    return request.contains("id") && (request["id"].is_number() || request["id"].is_string() || request["id"].is_null());
  }

} // namespace jsonrpccxx
