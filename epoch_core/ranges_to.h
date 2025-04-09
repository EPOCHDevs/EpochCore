#pragma once

#include <ranges>
#include <iterator>
#include <type_traits>
#include <vector>
#include <unordered_set>
#include <string>
#include <sstream>

namespace epoch_core {
namespace ranges {

// Helper to get the value type of a range
template<typename R>
using range_value_t = typename std::decay<decltype(*std::begin(std::declval<R&>()))>::type;

// Type traits to check if we can construct a container from iterators
template<typename Container, typename Range, typename = void>
struct is_constructible_from_iterators : std::false_type {};

template<typename Container, typename Range>
struct is_constructible_from_iterators<
    Container, Range,
    typename std::void_t<decltype(Container(std::begin(std::declval<Range&>()), 
                                  std::end(std::declval<Range&>())))>
> : std::true_type {};

// Type traits to check if a container has range insert method
template<typename Container, typename Range, typename = void>
struct has_range_insert : std::false_type {};

template<typename Container, typename Range>
struct has_range_insert<
    Container, Range,
    typename std::void_t<decltype(std::declval<Container&>().insert(
                std::declval<Container&>().end(), 
                std::begin(std::declval<Range&>()), 
                std::end(std::declval<Range&>())))>
> : std::true_type {};

// Type traits to check if a container has push_back
template<typename Container, typename Value, typename = void>
struct has_push_back : std::false_type {};

template<typename Container, typename Value>
struct has_push_back<
    Container, Value,
    typename std::void_t<decltype(std::declval<Container&>().push_back(std::declval<Value>()))>
> : std::true_type {};

// Helper function for containers without range constructor or insert
template<typename Container, typename Range>
Container to_container_fallback(Range&& range) {
    Container container;
    if constexpr (has_push_back<Container, range_value_t<Range>>::value) {
        for (auto&& item : range) {
            container.push_back(std::forward<decltype(item)>(item));
        }
    } else {
        // For containers with insert like std::set, std::map
        for (auto&& item : range) {
            container.insert(container.end(), std::forward<decltype(item)>(item));
        }
    }
    return container;
}

// Main implementation of 'to'
template<typename Container, typename Range>
Container to(Range&& range) {
    if constexpr (is_constructible_from_iterators<Container, Range>::value) {
        return Container(std::begin(range), std::end(range));
    } else if constexpr (has_range_insert<Container, Range>::value) {
        Container container;
        container.insert(container.end(), std::begin(range), std::end(range));
        return container;
    } else {
        return to_container_fallback<Container>(std::forward<Range>(range));
    }
}

// to with deduction guide for containers
template<template<typename...> class Container, typename Range, typename... Args>
auto to(Range&& range) {
    using value_type = range_value_t<Range>;
    return to<Container<value_type, Args...>>(std::forward<Range>(range));
}

// Convenience to_vector function
template<typename Range>
auto to_vector(Range&& range) {
    return to<std::vector<range_value_t<Range>>>(std::forward<Range>(range));
}

// Tag struct for pipe syntax
template<template<typename...> class Container, typename... Args>
struct to_t {
    template<typename Range>
    friend auto operator|(Range&& range, to_t) {
        return to<Container, Range, Args...>(std::forward<Range>(range));
    }
};

// Tag struct for to_vector pipe syntax
struct to_vector_t {
    template<typename Range>
    friend auto operator|(Range&& range, to_vector_t) {
        return to_vector(std::forward<Range>(range));
    }
};

// Create instances of the tag structs
template<template<typename...> class Container, typename... Args>
inline constexpr to_t<Container, Args...> to_v{};

inline constexpr to_vector_t to_vector_v{};

    // Convenience to_unordered_set function
    template<typename Range>
    auto to_unordered_set(Range&& range) {
        return to<std::unordered_set<range_value_t<Range>>>(std::forward<Range>(range));
    }

    // Tag struct for to_unordered_set pipe syntax
    struct to_unordered_set_t {
        template<typename Range>
        friend auto operator|(Range&& range, to_unordered_set_t) {
            return to_unordered_set(std::forward<Range>(range));
        }
    };

    // Create instance of the to_unordered_set tag struct
    inline constexpr to_unordered_set_t to_unordered_set_v{};

    // Convenience to_string function (joins elements with delimiter)
    template<typename Range>
    auto to_string(Range&& range, const std::string& delimiter = "") {
        std::ostringstream oss;
        bool first = true;
        for (auto&& item : range) {
            if (!first) {
                oss << delimiter;
            }
            oss << item;
            first = false;
        }
        return oss.str();
    }

    // Tag struct for to_string pipe syntax
    struct to_string_t {
        std::string delimiter;

        explicit to_string_t(std::string delimiter = "") : delimiter(std::move(delimiter)) {}

        template<typename Range>
        friend auto operator|(Range&& range, const to_string_t& tag) {
            return to_string(std::forward<Range>(range), tag.delimiter);
        }
    };

    // Create instance of the to_string tag struct (with empty delimiter by default)
    inline constexpr to_string_t to_string_v{};

    // Helper function to create a to_string_t with a specific delimiter
    inline to_string_t with_delimiter(std::string delimiter) {
        return to_string_t(std::move(delimiter));
    }


} // namespace ranges
} // namespace epoch_core