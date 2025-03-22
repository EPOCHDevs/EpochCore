//
// Created by adesola on 1/3/25.
//

#pragma once
#include "error_type.h"
#include <boost/date_time/posix_time/posix_time.hpp>


namespace epoch {
struct ErrorContext {
  EpochCoreErrorType error{EpochCoreErrorType::Null};
  std::string message{};  // Detailed error message
  std::unordered_map<std::string, std::string> additionalContext{};  // Extra details
  std::string timestamp{boost::posix_time::to_iso_extended_string(
      boost::posix_time::microsec_clock::universal_time())};  // ISO-8601 timestamp
};

}
