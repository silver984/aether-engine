#pragma once

#include "math-impl/type_concepts.hh"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace aether {

template <_math_impl::numeric_type_ T>
[[nodiscard]] constexpr T lerp(T const& a, T const& b, T t) {
	return a + (b - a) * t;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T lerp(T const& a, T const& b, typename T::_value_type const& t) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = a[i] + (b[i] - a[i]) * t;
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] T damp(T const& a, T const& b, typename T::_value_type const& l, typename T::_value_type const& dt) {
	return lerp(a, b, typename T::_value_type{1} - std::exp(-l * dt));
}

template <_math_impl::numeric_type_ T>
[[nodiscard]] constexpr int sign(T const& val) {
	return (val > T{0}) - (val < T{0});
}

template <_math_impl::numeric_type_ T>
[[nodiscard]] constexpr T avg(T const& a, T const& b) {
	return (a + b) / T{2};
}

template <_math_impl::numeric_type_ T>
[[nodiscard]] constexpr T map(T const& val, T const& in_min, T const& in_max, T const& out_min, T const& out_max) {
	return out_min + (out_max - out_min) * ((val - in_min) / (in_max - in_min));
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T clamp(T const& val, T min_val, T max_val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::clamp(val[i], min_val[i], max_val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] T round(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::round(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T ceil(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::ceil(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] T floor(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::floor(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T abs(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::abs(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] T sin(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::sin(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] T cos(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::cos(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] T tan(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::tan(val[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T min(T const& left, T const& right) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::min(left[i], right[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T max(T const& left, T const& right) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = std::max(left[i], right[i]);
	}
	return out;
}

template <_math_impl::indexable_math_type_ T>
[[nodiscard]] constexpr T reverse(T const& val) {
	T out;
	for (size_t i = 0; i < T::capacity(); ++i) {
		out[i] = val[T::capacity() - i - 1];
	}
	return out;
}

template <_math_impl::matching_capacity_math_type_<2> T>
[[nodiscard]] typename T::_value_type length(T const& val) {
	return std::sqrt(val[0] * val[0] + val[1] * val[1]);
}

template <_math_impl::matching_capacity_math_type_<2> T>
[[nodiscard]] T normalize(T const& val) {
	using value_type     = typename T::_value_type;
	value_type const len = length(val);
	return len == value_type{0} ? T{}
	                            : T{
	                                      val[0] / len,
	                                      val[1] / len,
	                              };
}

template <_math_impl::matching_capacity_math_type_<2> T>
[[nodiscard]] typename T::_value_type distance(T const& a, T const& b) {
	return std::sqrt((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}

template <_math_impl::matching_capacity_math_type_<2> T>
[[nodiscard]] constexpr typename T::_value_type dot(T const& a, T const& b) {
	return a[0] * b[0] + a[1] * b[1];
}

template <_math_impl::numeric_type_ T>
[[nodiscard]] constexpr T deg2rad(T const& deg) {
	return deg * (std::numbers::pi_v<T> / T{180});
}

template <_math_impl::numeric_type_ T>
[[nodiscard]] constexpr T rad2deg(T const& rad) {
	return rad * (T{180} / std::numbers::pi_v<T>);
}

} // namespace aether