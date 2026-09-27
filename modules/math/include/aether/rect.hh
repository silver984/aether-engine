#pragma once

#include "math-impl/simple_math_type.hh"
#include "size.hh"
#include "vec2.hh"

#include <utility>

namespace aether {

template <_math_impl::numeric_type_ T>
struct rect final : public _math_impl::simple_math_type_<rect, T, 4> {
	constexpr rect() = default;

	constexpr rect(T v)
	        : x(v)
	        , y(v)
	        , width(v)
	        , height(v) {}

	constexpr rect(T xv, T yv, T wv, T hv)
	        : x(xv)
	        , y(yv)
	        , width(wv)
	        , height(hv) {}

	[[nodiscard]] constexpr vec2<T> position() const { return {x, y}; }
	[[nodiscard]] constexpr size<T> bounds() const { return {width, height}; };

	constexpr T const& operator[](uint8_t i) const {
		switch (i) {
		case 0: {
			return x;
		}
		case 1: {
			return y;
		}
		case 2: {
			return width;
		}
		case 3: {
			[[fallthrough]];
		}
		default: {
			return height;
		}
		}
	}

	constexpr T& operator[](uint8_t i) { return const_cast<T&>(std::as_const(*this)[i]); }

	T x      = T{0};
	T y      = T{0};
	T width  = T{0};
	T height = T{0};
};

} // namespace aether