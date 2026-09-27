#pragma once

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace aether::_math_impl {

template <typename T_>
concept numeric_type_ = std::is_arithmetic_v<T_>;

template <typename T_>
concept indexable_math_type_ = numeric_type_<typename T_::_value_type> && requires(T_ v, T_ const cv, uint8_t i) {
	{ T_::capacity() } -> std::same_as<uint8_t>;
	{ v[i] } -> std::same_as<typename T_::_value_type&>;
	{ cv[i] } -> std::same_as<typename T_::_value_type const&>;
};

template <typename T_, uint8_t N_>
concept matching_capacity_math_type_ = indexable_math_type_<T_> && requires { requires T_::capacity() == N_; };

} // namespace aether::_math_impl