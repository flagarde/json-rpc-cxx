#pragma once
#include "nlohmann/json.hpp"
#include <exception>
#include <string>

namespace jsonrpccxx {
  typedef nlohmann::json json;

  enum error_type {
    parse_error = -32700,
    invalid_request = -32600,
    method_not_found = -32601,
    invalid_params = -32602,
    internal_error = -32603,
    server_error,
    invalid
  };

  class JsonRpcException : public std::exception {
  public:
    JsonRpcException(int code, const std::string &message) noexcept : code(code), message(message), data(nullptr), err(std::to_string(code) + ": " + message) {}
    JsonRpcException(int code, const std::string &message, const nlohmann::json &data) noexcept
        : code(code), message(message), data(data), err(std::to_string(code) + ": " + message + ", data: " + data.dump()) {}

    error_type Type() const {
      if (code >= -32603 && code <= -32600)
        return static_cast<error_type>(code);
      if (code >= -32099 && code <= -32000)
        return server_error;
      if (code == -32700)
        return parse_error;
      return invalid;
    }

    int Code() const { return code; }
    const std::string& Message() const { return message; }
    const json &Data() const { return data; }

    const char* what() const noexcept override { return err.c_str(); }

    static inline JsonRpcException fromJson(const nlohmann::json &value)
    {
      if (value.contains("code") && value["code"].is_number_integer() && value.contains("message") && value["message"].is_string())
      {
        if (value.contains("data")) {
          return JsonRpcException(value["code"], value["message"], value["data"].get<json>());
        } else {
          return JsonRpcException(value["code"], value["message"]);
        }
      }
      return JsonRpcException(internal_error, R"(invalid error response: "code" (integer number) and "message" (string) are required)");
    }

  private:
    int code;
    std::string message;
    nlohmann::json data;
    std::string err;
  };
} // namespace jsonrpccxx
