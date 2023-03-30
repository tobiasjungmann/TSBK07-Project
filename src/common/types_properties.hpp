#pragma once

#include <type_traits>

namespace scn {
struct NonMoveable {
    constexpr NonMoveable() = default;
    constexpr NonMoveable& operator=(NonMoveable&&) = delete;
    constexpr NonMoveable(NonMoveable&&) = delete;
};

struct NonCopyable {
    constexpr NonCopyable() = default;
    constexpr NonCopyable& operator=(NonCopyable&&) = delete;
    constexpr NonCopyable(NonCopyable&&) = delete;
};

template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T value) noexcept {
    return static_cast<std::underlying_type_t<T>>(value);
}
}