#pragma once

#include <type_traits>

namespace aether::_math_impl {
template <typename T_>
concept numeric_type_ = std::is_arithmetic_v<T_>;
} // namespace aether::_math_impl