//
// Created by dewe on 9/3/21.
//
#pragma once

#include "uuid_v4.h"
#include <cmath>
#include <chrono>
#include <format>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <filesystem>
#include "macros.h"
#include <optional>
#include <random>
#include <ranges>
#include <unordered_map>
#include <variant>


namespace epoch {
    template<template<class, class...> class Container, class T, class ... Args>
    std::string toString(Container<T, Args ...> const &x) {
        std::ostringstream ss;
        ss << "[";
        std::ranges::copy(x, std::ostream_iterator<T>(ss, " "));
        ss << "]";
        return ss.str();
    }

    std::string GetEnvVariableOrThrow(std::string const &key, std::string const &defaultValue = "");

    template<typename T, typename Fn, typename Container>
    std::vector<T> Select(Container const &container, Fn &&fn) {
        std::vector<T> result;
        std::ranges::transform(container, std::back_inserter(result), fn);
        return result;
    }

    template<typename T, typename U>
    bool AllOfEquals(std::vector<T> const &container, U expectedValue) {
        return std::ranges::all_of(
                container,
                [=](auto currentValue) { return currentValue == expectedValue; });
    }

    template<typename T, typename U>
     bool NoneOfEquals(std::vector<T> const &container, U expectedValue) {
        return std::ranges::none_of(
                container,
                [=](auto currentValue) { return currentValue == expectedValue; });
    }

    bool isInternetConnected();

    inline std::string GenerateUUID() {
        return UUIDv4::UUIDGenerator<std::mt19937_64>{}.getUUID().str();
    }

    inline std::string now() {
        auto now = std::chrono::system_clock::now();
        return std::format("{:%Y-%m-%d %H:%M:%S}", now);
    }

    template<typename... T>
    inline double GetDouble(const std::variant<T...> &args) {
        return std::get<double>(args);
    }

    template<typename... T>
    inline int GetInt(const std::variant<T...> &args) {
        return std::get<int64_t>(args);
    }

    template<class T>
    inline auto negate_if_true(bool condition, T const &val) {
        return condition ? -val : val;
    }

    template<template<class ...> class ContainerType, class ... Args, class KeyType, class MessageType=std::string>
    static auto &lookup(ContainerType<Args ...> const &container,
                        KeyType const &key,
                        MessageType const &error_msg = MessageType{}) {
        try {
            return container.at(key);
        }
        catch (std::out_of_range const &exp) {
            std::stringstream ss;

            if constexpr (not std::is_same_v<std::tuple_element_t<0, std::tuple<Args ...>>, KeyType>) {
                ss << "IndexOutOfRange: \"" << key << "\", MaxLength: " << container.size();
            } else {
                ss << "KeyNotFound: \"" << key << "\"\nAvailable Keys:\n";
                for (auto const &x: container | std::views::keys) {
                    ss << x << "\n";
                }
            }

            ss << "\nmsg: " << error_msg << "\n";
            SPDLOG_ERROR(ss.str());
            throw;
        }
    }

    template<template<class ...> class ContainerType, class ... Args, class KeyType, class ValueType>
    static auto lookupDefault(ContainerType<Args ...> const &container,
                              KeyType const &key,
                              ValueType const &defaultValue) {
        try {
            return container.at(key);
        }
        catch (std::out_of_range const &exp) {
            return defaultValue;
        }
    }

    template<template<class ...> class ContainerType, class ... Args, class KeyType>
    static const typename ContainerType<Args ...>::mapped_type *lookupDefaultPtr(ContainerType<Args ...> const &container,
                                                                        KeyType const &key) {
        try {
            return &container.at(key);
        }
        catch (std::out_of_range const &exp) {
            return nullptr;
        }
    }

#define LAMBDA(fnc) [&](auto const& x) { fnc(x); }
#define LAMBDA_WITH_RETURN(fnc) [&](auto const& x) { return fnc(x); }

    template<template<class, class, class ...> class Container, class Key, class Value, class ... Args>
    std::vector<Value> GetValuesAsList(Container<Key, Value, Args ...> const &container) {
        std::vector<Value> result;
        result.reserve(container.size());
        std::ranges::copy(container | std::views::values, std::back_inserter(result));
        return result;
    }

    template<class T>
    std::string OptionalToString(std::optional<T> const &optionalValue) {
        if (optionalValue) {
            return TO_STRING(*optionalValue);
        } else {
            return "None.";
        }
    }

    template<class T>
    inline std::vector<T> merge(std::vector<T> const &a, std::vector<T> const &b) {
        std::vector<T> result;
        result.reserve(a.size() + b.size());
        result.insert(result.end(), a.begin(), a.end());
        result.insert(result.end(), b.begin(), b.end());
        return result;
    }

}
