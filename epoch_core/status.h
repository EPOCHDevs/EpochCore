//
// Created by adesola on 6/20/24.
//

#pragma once

#include <string>
#include "error_context.h"

// TODO: make_error_context template
// TODO MAKE_STATUS CODE TEMPLATE ENUM
namespace epoch_core {
  struct Status {
    bool isOk{true};
    ErrorContext error{};
    uint32_t status{};

    explicit operator bool() const { return isOk; }


    static Status failed(ErrorContext const &error, uint32_t status = 400) {
      return {false, error, status};
    }

    bool operator==(const Status & other) const {
      return other.status == status && other.isOk == isOk && (
      error.error == other.error.error &&
      error.additionalContext == other.error.additionalContext &&
      error.message == other.error.message
      );
    }

    friend std::ostream& operator<< (std::ostream& stream, const Status& status) {
      stream << "Status(isOk=" << std::boolalpha << status.isOk;
      stream << ", status=" << status.status << ", error={error=" << EpochCoreErrorTypeWrapper::ToString(status.error.error)
      << ", message=" << status.error.message << "})";
      return stream;
    }
  };

  inline Status MakeOkStatus() { return {true}; }

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
