#pragma once
//
// Created by dewe on 1/8/23.
//
//#include <boost/exception/all.hpp>
#include "sstream"
#include "exception"
#include <spdlog/spdlog.h>


#define EXPAND(iter) iter.begin(), iter.end()
#define TO_STRING(_txt) \
    [&]() \
    { \
        std::stringstream _ss; \
        _ss << _txt; \
        return _ss.str(); \
    }()

#define ADD_ACCESSORS(suffix, attribute) \
    inline auto Get##suffix() const \
    { \
        return this->attribute; \
    }

#define ADD_ACCESSORS_AND_MUTATORS(suffix, attribute) \
    template<typename T> \
    inline auto& Set##suffix(T&& x) \
    { \
        this->attribute = std::forward<T>(x); \
        return *this; \
    } \
    template<typename... T> \
    inline auto& Emplace##suffix(T&&... x) \
    { \
        this->attribute = { std::forward<T>(x)... }; \
        return *this; \
    } \
\
    inline auto Get##suffix() const \
    { \
        return this->attribute; \
    }

#define ADD_ACCESSORS_AND_MUTATORS_CONTAINER(suffix, attribute) \
    ADD_ACCESSORS_AND_MUTATORS(suffix, attribute) \
    inline void AppendTo##suffix(auto const& value) \
    { \
        this->attribute.insert(this->attribute.end(), value); \
    }

#define ADD_ACCESSORS_AND_MUTATORS_SET(suffix, attribute) \
    ADD_ACCESSORS_AND_MUTATORS(suffix, attribute) \
    inline void AppendTo##suffix(auto const& value) \
    { \
        this->attribute.insert(value); \
    }

#define ADD_ACCESSORS_AND_MUTATORS_BOOL(suffix, attribute) \
    template<typename T> \
    inline auto& Set##suffix(T&& x) \
    { \
        this->attribute = std::forward<T>(x); \
        return *this; \
    } \
\
    inline bool Is##suffix() const \
    { \
        return this->attribute; \
    }

#define DEBUG_INFO_STR \
    std::string("\nFile: ").append(__FILE__) \
    .append("\nLine: ").append(std::to_string(__LINE__)) \
    .append("\nFunction: ").append(__FUNCTION__)

#define FORMAT_EXCEPTION_MESSAGE(msg) \
    [&]() { \
        std::stringstream sstr; \
        sstr << msg << DEBUG_INFO_STR; \
        return sstr.str(); \
    }()

#define ThrowExceptionImpl(ExceptionClass, info_str, ...) \
    SPDLOG_THROW(ExceptionClass(std::format(info_str, __VA_ARGS__)))

#define ThrowException(info_str, ...) \
    do { \
        std::string msg = std::format(info_str, __VA_ARGS__); \
        SPDLOG_THROW(std::runtime_error(FORMAT_EXCEPTION_MESSAGE(msg))); \
    } while(0)

#define ThrowExceptionFromFormatImpl(ExceptionClass, info_str, ...) \
    do { \
        std::string msg = std::format(info_str, __VA_ARGS__); \
        SPDLOG_THROW(ExceptionClass(FORMAT_EXCEPTION_MESSAGE(msg))); \
    } while(0)

#define ThrowExceptionFromFormat(info_str, ...) \
    ThrowExceptionFromFormatImpl(std::runtime_error, info_str, __VA_ARGS__)

#define ThrowExceptionFromStreamImpl(ExceptionClass, info_str) \
    do { \
        std::stringstream sstr; \
        sstr << info_str; \
        SPDLOG_THROW(ExceptionClass(FORMAT_EXCEPTION_MESSAGE(sstr.str()))); \
    } while(0)

#define ThrowExceptionFromStream(info_str) \
    ThrowExceptionFromStreamImpl(std::runtime_error, info_str)

#define AssertFromStreamImpl(ExceptionClass, cond, message_stream) \
    do { \
        if (NOT(cond)) { \
            std::ostringstream _oss_assert; \
            _oss_assert << "Assertion failed: " << #cond << "\nMessage: " << message_stream; \
            std::string _msg_assert = _oss_assert.str(); \
            SPDLOG_THROW(ExceptionClass(FORMAT_EXCEPTION_MESSAGE(_msg_assert))); \
        } \
    } while(0)

#define AssertFromStream(cond, message_stream) \
    do { \
        if (NOT(cond)) { \
            std::ostringstream _oss_assert; \
            _oss_assert << "Assertion failed: " << #cond << "\nMessage: " << message_stream; \
            std::string _msg_assert = _oss_assert.str(); \
            SPDLOG_THROW(std::runtime_error(FORMAT_EXCEPTION_MESSAGE(_msg_assert))); \
        } \
    } while(0)

#define AssertFalseFromStreamImpl(ExceptionClass, cond, message_stream) \
    do { \
        if (cond) { \
            std::ostringstream _oss_assert; \
            _oss_assert << "False assertion failed: " << #cond << "\nMessage: " << message_stream; \
            std::string _msg_assert = _oss_assert.str(); \
            SPDLOG_THROW(ExceptionClass(FORMAT_EXCEPTION_MESSAGE(_msg_assert))); \
        } \
    } while(0)

#define AssertFalseFromStream(cond, message_stream) \
    do { \
        if (cond) { \
            std::ostringstream _oss_assert; \
            _oss_assert << "False assertion failed: " << #cond << "\nMessage: " << message_stream; \
            std::string _msg_assert = _oss_assert.str(); \
            SPDLOG_THROW(std::runtime_error(FORMAT_EXCEPTION_MESSAGE(_msg_assert))); \
        } \
    } while(0)

#define NOT(x) (!(x))


#define AssertFromFormatImpl(ExceptionClass, cond, info_str, ...) \
    do { \
        if (NOT(cond)) { \
            std::string errorMsg = std::format(info_str, ##__VA_ARGS__); \
            std::stringstream sstr; \
            sstr << "Assertion failed: " << #cond << "\nMessage: " << errorMsg; \
            SPDLOG_THROW(ExceptionClass(FORMAT_EXCEPTION_MESSAGE(sstr.str()))); \
        } \
    } while(0)


#define AssertFalseFromFormatImpl(ExceptionClass, cond, info_str, ...) \
    do { \
        if (cond) { \
            std::string errorMsg = std::format(info_str, ##__VA_ARGS__); \
            std::stringstream sstr; \
            sstr << "False assertion failed: " << #cond << "\nMessage: " << errorMsg; \
            SPDLOG_THROW(ExceptionClass(FORMAT_EXCEPTION_MESSAGE(sstr.str()))); \
        } \
    } while(0)

#define AssertFromFormat(cond, info_str, ...) \
    AssertFromFormatImpl(std::runtime_error, cond, info_str, ##__VA_ARGS__)

#define AssertFalseFromFormat(cond, info_str, ...) \
    AssertFalseFromFormatImpl(std::runtime_error, cond, info_str, ##__VA_ARGS__)

#define CHECK_DECIMAL_INTEGRITY(decimal_value) \
    AssertFromStream(decimal_value.isnormal() && decimal_value > 0, \
        #decimal_value << " Got " << decimal_value)

#define SPDLOG_DEBUG_IF(condition, msg, ...) \
do                                           \
{                                        \
        if constexpr(SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG) \
        { \
            if (condition) spdlog::debug(msg, __VA_ARGS__); \
        }                                            \
}while(0)


#define SPDLOG_INFO_IF(condition, msg, ...) \
do                                           \
{                                        \
            if (condition) spdlog::info(msg, __VA_ARGS__); \
}while(0)


#define ADD_MAKER(ClassName) \
    template<typename... T> \
    inline static std::unique_ptr<ClassName> Make(T&&... args) \
    { \
        return std::make_unique<ClassName>(std::forward<T>(args)...); \
    }


#define VARIANT_CLASS_MEMBERS(ClassName, VariantType)\
ClassName(VariantType x) : impl(std::move(x)) {}\
inline bool Is##VariantType() const { return std::holds_alternative<VariantType>(impl); }\
inline VariantType Get##VariantType() const { return std::get<VariantType>(impl); }\
inline VariantType* Get##VariantType##Ptr() { return std::get_if<VariantType>(&impl); }

#define VARIANT_STREAMING_FEATURE(ClassName)\
inline friend std::ostream& operator<<(std::ostream& streamer, const ClassName& obj) \
    { std::visit([&streamer](const auto& element) { streamer << element; }, obj.impl); return streamer; }

#define OPTIONAL_VALUE_OR(x, TrueValue, FalseValue) (x ? TrueValue : FalseValue)
#define OPTIONAL_VALUE_OR_TRUE(x, TrueValue) (OPTIONAL_VALUE_OR(x, TrueValue, true))


#define THROW_IMPLEMENTATION_ERROR(ClassName) \
    do { \
        std::stringstream sstr; \
        sstr << "Implementation error in class: " << #ClassName; \
        SPDLOG_THROW(std::runtime_error(FORMAT_EXCEPTION_MESSAGE(sstr.str()))); \
    } while(0)

#define CLASS_LAMBDA(FUNCTOR) [this](auto const& x) { return FUNCTOR(x); }
#define REF_LAMBDA(FUNCTOR) [&](auto const& x) { return FUNCTOR(x); }
#define MEMBER_LAMBDA(FUNCTOR) [&](auto const& x) { return x.FUNCTOR; }

#define CONST_EXPR_TERNARY_FUNCTION(Condition, TTruth, TFalse) \
    if constexpr (Condition) { return TTruth; } else { return TFalse; }