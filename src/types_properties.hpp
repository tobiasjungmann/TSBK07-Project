#pragma once

#include <type_traits>


#define DELETE_MOVE_SEMANTIC(class) \
    constexpr #class & operator=(#class &&) = delete; \
    constexpr #class (#class &&) = delete;

#define DELETE_COPY_SEMANTIC(class) \
    constexpr #class & operator=(#class const&) = delete; \
    constexpr #class (#class const&) = delete; \
    constexpr ~#class() = default;


template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T value) noexcept {
    return static_cast<std::underlying_type_t<T>>(value);
}


template <typename From, typename To>
using Castable = std::enable_if_t<std::is_convertible_v<From, To>>;