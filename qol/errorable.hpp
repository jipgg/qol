#pragma once
#include <optional>
#include <variant>
#include <format>
#include <iostream>
#include <source_location>

namespace qol {
struct noerror_t {
    [[nodiscard]] explicit constexpr noerror_t() = default;
};
constexpr noerror_t noerror{};
template <class Error, class Return = noerror_t>
class errorable {
    std::variant<Return, Error> var_;
    bool has_value_;
    std::source_location source_location_;
    mutable bool error_handled_ = false;
public:
    ~errorable() noexcept {
        if (!has_value_ and !error_handled_) unhandled_error("unhandled error", source_location_);
    }
    template <class ...Init>
    [[nodiscard]] constexpr errorable(std::in_place_type_t<Return>, Init&&...args,
                                      const std::source_location& sl = std::source_location::current()) noexcept:
        var_(std::in_place_type<Return>, std::forward<Init>(args)...),
        has_value_(true),
        source_location_(sl) {
    }
    template <class ...Init>
    [[nodiscard]] constexpr errorable(std::in_place_type_t<Error>, Init&&...args,
                                      const std::source_location& sl = std::source_location::current()) noexcept:
        var_(std::in_place_type<Error>, std::forward<Init>(args)...),
        has_value_(false),
        source_location_(sl) {
    }
    [[nodiscard]] explicit constexpr errorable(const errorable& other) = default;
    [[nodiscard]] constexpr errorable& operator=(const errorable& other) = default;
    [[nodiscard]] explicit constexpr errorable(errorable&& other) noexcept:
        var_(std::move(other.var_)),
        has_value_(other.has_value_),
        source_location_(std::move(other.source_location_)),
        error_handled_(other.error_handled_) {
        other.ignore();
    }
    [[nodiscard]] constexpr errorable& operator=(errorable&& other) noexcept {
        var_ = std::move(other.var_);
        has_value_ = other.has_value_;
        source_location_ = std::move(other.source_location_);
        error_handled_ = other.error_handled_;
        other.ignore();
    }
    [[nodiscard]] constexpr errorable(const Return& value,
                                               const std::source_location& sl = std::source_location::current()) noexcept:
        var_(value),
        has_value_(true),
        source_location_(sl) {
    }
    [[nodiscard]] constexpr errorable(const Error& error,
                                               const std::source_location& sl = std::source_location::current()) noexcept:
        var_(error),
        has_value_(false),
        source_location_(sl) {
    }
    [[nodiscard]] constexpr errorable(Return&& value,
                                               const std::source_location& sl = std::source_location::current()) noexcept:
        var_(std::move(value)),
        has_value_(true),
        source_location_(sl) {
    }
    [[nodiscard]] constexpr errorable(Error&& error,
                                               const std::source_location& sl = std::source_location::current()) noexcept:
        var_(std::move(error)),
        has_value_(false),
        source_location_(sl) {
    }
    [[nodiscard]] constexpr const Return& value() const noexcept {
        if (!has_value_) unhandled_error("value is error", source_location_);
        return std::get<Return>(var_);
    }
    [[nodiscard]] constexpr const Error& error() const noexcept {
        if (has_value_) unhandled_error("error is value", source_location_);
        error_handled_ = true;
        return std::get<Error>(var_);
    }
    [[nodiscard]] constexpr Return& value() noexcept {
        if (!has_value_) unhandled_error("value is error", source_location_);
        return std::get<Return>(var_);
    }
    [[nodiscard]] constexpr Error& error() noexcept {
        if (has_value_) unhandled_error("error is value", source_location_);
        error_handled_ = true;
        return std::get<Error>(var_);
    }
    [[nodiscard]] constexpr operator bool() const noexcept {
        return has_value_;
    }
    [[nodiscard]] constexpr bool success() const noexcept {
        return has_value_;
    }
    constexpr void ignore() noexcept {
        error_handled_ = true;
    }
    [[nodiscard]] constexpr const Return& or_else(const Return& def) {
        return has_value_ ? std::get<Return>(var_) : def;
    }
    [[nodiscard]] constexpr Return& operator*() {
        return value();
    }
    [[nodiscard]] constexpr Return* operator->() {
        return &value();
    }
    [[nodiscard]] constexpr bool is_value() const {
        return has_value_;
    }
    [[nodiscard]] constexpr bool is_error() const {
        return not has_value_;
    }
private:
    [[noreturn]] void unhandled_error(std::string_view description) noexcept {
        std::cerr << std::format(
        "{}: [{}:{}] {}\nin function: {}",
            description,
            source_location_.line(),
            source_location_.column(),
            source_location_.file_name(),
            source_location_.function_name()
        );
        std::abort();
    }
};
}
