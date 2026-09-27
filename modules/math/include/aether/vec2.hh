#pragma once

#include "math-impl/simple_math_type.hh"

namespace aether {

template <_math_impl::numeric_type_ T>
struct vec2 final : public _math_impl::simple_math_type_<vec2, T, 2> {
	constexpr vec2() = default;

	constexpr vec2(T v)
	        : x(v)
	        , y(v) {}

	constexpr vec2(T xv, T yv)
	        : x(xv)
	        , y(yv) {}

	constexpr T& operator[](uint8_t i) {
		switch (i) {
		case 0: {
			return x;
		}
		case 1: {
			[[fallthrough]];
		}
		default: {
			return y;
		}
		}
	}

	constexpr T const& operator[](uint8_t i) const { return const_cast<vec2&>(*this)[i]; }

	T x = T{0};
	T y = T{0};
};

} // namespace aether