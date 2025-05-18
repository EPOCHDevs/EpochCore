#pragma once
#include "macros.h"
#include <algorithm>
#include <array>
#include <glaze/glaze.hpp>
#include <map>
#include <ranges>
#include <string>
#include <vector>

namespace epoch_core {
// Custom trim functions
constexpr std::string_view trim_left(std::string_view sv) {
  sv.remove_prefix(std::min(sv.find_first_not_of(" \t\n\r\f\v"), sv.size()));
  return sv;
}

constexpr std::string_view trim_right(std::string_view sv) {
  auto pos = sv.find_last_not_of(" \t\n\r\f\v");
  if (pos != std::string_view::npos) {
    sv.remove_suffix(sv.size() - pos - 1);
  } else {
    sv = {};
  }
  return sv;
}

constexpr std::string_view trim(std::string_view sv) {
  return trim_left(trim_right(sv));
}

// TODO: Scope and REFACTOR
template <typename EnumClass, const char *str>
std::pair<std::map<std::string, EnumClass>, std::map<EnumClass, std::string>>
Parse() {
  std::map<EnumClass, std::string> stringEnum;
  std::map<std::string, EnumClass> enumAsString;

  int numValue = 0;

  // Use C++23 views::split with string_view
  for (const auto &element : std::string_view{str} | std::views::split(',')) {
    std::string_view elementView{element.begin(), element.end()};
    elementView = trim(elementView); // Use custom trim function

    if (auto value_pos = elementView.find('=');
        value_pos != std::string_view::npos) {
      // Has explicit value
      auto name = elementView.substr(0, value_pos);
      auto value = elementView.substr(value_pos + 1);
      name = trim(name);   // Use custom trim function
      value = trim(value); // Use custom trim function

      // Correctly parse the integer value
      int8_t parsedValue = 0;
      const auto result = std::from_chars(
          value.data(), value.data() + value.size(), parsedValue);
      if (result.ec == std::errc()) {
        numValue = parsedValue;
      }

      stringEnum[static_cast<EnumClass>(numValue)] = std::string{name};
      enumAsString[std::string{name}] = static_cast<EnumClass>(numValue);
    } else {
      // No explicit value
      stringEnum[static_cast<EnumClass>(numValue)] = std::string{elementView};
      enumAsString[std::string{elementView}] = static_cast<EnumClass>(numValue);
      ++numValue;
    }
  }

  // Add Null value
  stringEnum[EnumClass::Null] = "Null";
  enumAsString["Null"] = EnumClass::Null;

  return std::pair{enumAsString, stringEnum};
}

template <class EnumClass, class NumType, const char *EnumClassAsStr,
          const char *elements>
struct EnumClassT {
  using type = EnumClass;
  constexpr static auto name = EnumClassAsStr;
  inline static const std::pair<std::map<std::string, EnumClass>,
                                std::map<EnumClass, std::string>>
      m_data{Parse<EnumClass, elements>()};

  static std::string EncodeEnumString(std::string const &enumClassAsString) {
    return isdigit(enumClassAsString[0]) ? "_" + enumClassAsString
                                         : enumClassAsString;
  }
  static std::string DecodeEnumString(std::string const &enumClassAsString) {
    return enumClassAsString[0] == '_' ? enumClassAsString.substr(1)
                                       : enumClassAsString;
  }

  static std::string ToString(EnumClass enumClass) {
    return DecodeEnumString(m_data.second.at(enumClass));
  }

  static auto GetAll() { return m_data.second | std::views::keys; }

  static auto GetAllAsStrings() { return m_data.first | std::views::keys; }

  static bool IsValid(std::string const &enumClassAsString) {
    return m_data.first.contains(enumClassAsString);
  }

  static EnumClass FromString(std::string const &enumClassAsString) {
    auto enumAsStringIt = m_data.first.find(enumClassAsString);
    if (enumAsStringIt == m_data.first.end()) {
      throw std::invalid_argument("Invalid enum value for " +
                                  std::string(EnumClassAsStr));
    }
    return enumAsStringIt->second;
  }

  static NumType toNumber(EnumClass enumClass) {
    return static_cast<NumType>(enumClass);
  }
};

template <class T> struct EnumWrapper;
} // namespace epoch_core

#define CREATE_ENUM_COMMON_CLASS(EnumClass, NumType, ...)                      \
  namespace epoch_core {                                                       \
  static const char EnumClass##Args[] = #__VA_ARGS__;                          \
  static const char __##EnumClass##AsStr__[] = #EnumClass;                     \
  enum class EnumClass : NumType { __VA_ARGS__, Null };                        \
                                                                               \
  using EnumClass##Wrapper =                                                   \
      EnumClassT<EnumClass, NumType, __##EnumClass##AsStr__, EnumClass##Args>; \
  template <> struct EnumWrapper<EnumClass> {                                  \
    using type = EnumClass##Wrapper;                                           \
  };                                                                           \
  const EnumClass##Wrapper EnumClass##Instance{};                              \
  }

#define ADD_GLAZE_ENUM(EnumClass, NumType, ...)                                \
  template <> struct glz::meta<epoch_core::EnumClass> {                        \
    using enum epoch_core::EnumClass;                                          \
    static constexpr auto value = enumerate(__VA_ARGS__);                      \
  }

#define CREATE_ENUM_COMMON(EnumClass, NumType, ...)                            \
  CREATE_ENUM_COMMON_CLASS(EnumClass, NumType, __VA_ARGS__);                   \
  namespace epoch_core {                                                       \
  inline std::ostream &operator<<(std::ostream &os, EnumClass enumClass) {     \
    os << EnumClass##Wrapper::ToString(enumClass);                             \
    return os;                                                                 \
  }                                                                            \
  inline constexpr bool IsValid(EnumClass const &enumClass) {                  \
    return enumClass != EnumClass::Null;                                       \
  }                                                                            \
  }

#define CREATE_ENUM(EnumClass, ...)                                            \
  CREATE_ENUM_COMMON(EnumClass, uint8_t, __VA_ARGS__)                          \
  ADD_GLAZE_ENUM(EnumClass, uint8_t, __VA_ARGS__)

#define CREATE_ENUM_SIGNED(EnumClass, ...)                                     \
  CREATE_ENUM_COMMON(EnumClass, int8_t, __VA_ARGS__)
