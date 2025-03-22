#pragma once
#include <glaze/glaze.hpp>
#include <map>
#include <string>
#include <vector>
#include "macros.h"
#include <ranges>
#include <array>
#include <algorithm>

// Custom trim functions
inline std::string_view trim_left(std::string_view sv) {
    sv.remove_prefix(std::min(sv.find_first_not_of(" \t\n\r\f\v"), sv.size()));
    return sv;
}

inline std::string_view trim_right(std::string_view sv) {
    auto pos = sv.find_last_not_of(" \t\n\r\f\v");
    if (pos != std::string_view::npos) {
        sv.remove_suffix(sv.size() - pos - 1);
    } else {
        sv = {};
    }
    return sv;
}

inline std::string_view trim(std::string_view sv) {
    return trim_left(trim_right(sv));
}

// TODO: Scope and REFACTOR

template <class EnumClass>
inline auto Parse(std::string_view str) {
    std::map<EnumClass, std::string> stringEnum;
    std::map<std::string, EnumClass> enumAsString;

    int8_t numValue = 0;
    
    // Use C++23 views::split with string_view
    for (const auto& element : str | std::views::split(',')) {
        std::string_view elementView{element.begin(), element.end()};
        elementView = trim(elementView);  // Use custom trim function
        
        if (auto value_pos = elementView.find('='); value_pos != std::string_view::npos) {
            // Has explicit value
            auto name = elementView.substr(0, value_pos);
            auto value = elementView.substr(value_pos + 1);
            name = trim(name);  // Use custom trim function
            value = trim(value);  // Use custom trim function
            
            // Correctly parse the integer value
            int8_t parsedValue = 0;
            auto result = std::from_chars(value.data(), value.data() + value.size(), parsedValue);
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

    return std::pair{stringEnum, enumAsString};
}

template <class EnumClass> struct EnumClassT {
  // using type = EnumClass;
  EnumClassT(const char *elements) {
    std::tie(stringEnum, enumAsString) = Parse<EnumClass>(elements);
  }

  static std::string EncodeEnumString(std::string const &enumClassAsString) {
    return isdigit(enumClassAsString[0]) ? "_" + enumClassAsString
                                         : enumClassAsString;
  }
  static std::string DecodeEnumString(std::string const &enumClassAsString) {
    return enumClassAsString[0] == '_' ? enumClassAsString.substr(1)
                                       : enumClassAsString;
  }

  static std::string ToString(EnumClass enumClass) {
    return DecodeEnumString(stringEnum.at(enumClass));
  }

  static auto GetAll() { return stringEnum | std::views::keys; }

  static auto GetAllAsStrings() { return enumAsString | std::views::keys; }

  static bool IsValid(std::string const &enumClassAsString) {
    return enumAsString.contains(enumClassAsString);
  }

protected:
  inline static std::map<EnumClass, std::string> stringEnum;
  inline static std::map<std::string, EnumClass> enumAsString;
};

template <class T> struct EnumWrapper;


#define CREATE_ENUM_COMMON_CLASS(EnumClass, NumType, ...) \
enum class EnumClass : NumType { __VA_ARGS__, Null }; \
\
struct EnumClass##Wrapper: EnumClassT<EnumClass> { \
using type = EnumClass; \
EnumClass##Wrapper(): EnumClassT<EnumClass>(#__VA_ARGS__)  {} \
    static  EnumClass FromString(std::string const& enumClassAsString) {\
auto enumAsStringIt = enumAsString.find(EncodeEnumString(enumClassAsString));\
AssertFalseFromStream(enumAsStringIt == enumAsString.end(), enumClassAsString << " is not a valid enum for " << #EnumClass);\
return enumAsStringIt->second;\
}                                                          \
static bool Is##EnumClass(std::string const& enumClassAsString) { return IsValid(enumClassAsString); }               \
static NumType toNumber(EnumClass enumClass) { return static_cast<NumType>(enumClass); }  \
};                                                        \
                                                          \
template<> \
struct EnumWrapper<EnumClass> { \
    using type = EnumClass##Wrapper; \
};                                    \
const EnumClass##Wrapper EnumClass##Instance{}

#define ADD_GLAZE_ENUM(EnumClass, NumType, ...) \
    template <> \
    struct glz::meta<EnumClass> { \
        using enum EnumClass; \
        static constexpr auto value = enumerate(__VA_ARGS__); \
    }                                                            \

#define CREATE_ENUM_COMMON(EnumClass, NumType, ...) \
    CREATE_ENUM_COMMON_CLASS(EnumClass, NumType, __VA_ARGS__); \
    inline std::ostream& operator<<(std::ostream& os, EnumClass enumClass) { \
        os << EnumClass##Wrapper::ToString(enumClass); \
        return os; \
    } \
    inline constexpr bool IsValid(EnumClass const& enumClass) { return enumClass != EnumClass::Null; }

#define CREATE_ENUM(EnumClass, ...) \
    CREATE_ENUM_COMMON(EnumClass, uint8_t, __VA_ARGS__); ADD_GLAZE_ENUM(EnumClass, uint8_t, __VA_ARGS__)

#define CREATE_ENUM_SIGNED(EnumClass, ...) CREATE_ENUM_COMMON(EnumClass, int8_t, __VA_ARGS__)
