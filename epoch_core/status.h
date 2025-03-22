//
// Created by adesola on 6/20/24.
//

#pragma once

#include <string>
#include "error_context.h"


namespace epoch {
struct Status {
  bool isOk{true};
  ErrorContext error{};
  uint32_t status{};

  explicit operator bool() const { return isOk; }

  static Status ok() { return {true}; }

  static Status failed(ErrorContext const &error, uint32_t status = 400) {
    return {false, error, status};
  }
};

template <class T> struct Result {
  Status status;
  T value{};

  explicit operator bool() const { return status.isOk; }

  static Result<T> failed(ErrorContext const &error,
                          uint32_t const &status = 400) {
    return {Status{false, error, status}};
  }
};
}

#define RETURN_IF_FAILED(status)             if (!status.isOk) return status;
