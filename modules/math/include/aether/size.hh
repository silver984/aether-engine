#pragma once

#include "math-impl/simple_math_type.hh"

#include <utility>

namespace aether {

template <_math_impl::numeric_type_ T>
struct size final : public _math_impl::simple_math_type_<size, T, 2> {
	constexpr size() = default;

	constexpr size(T v)
	        : width(v)
	        , height(v) {}

	constexpr size(T wv, T hv)
	        : width(wv)
	        , height(hv) {}

	constexpr T const& operator[](uint8_t i) const {
		switch (i) {
		case 0: {
			return width;
		}
		case 1: {
			[[fallthrough]];
		}
		default: {
			return height;
		}
		}
	}

	constexpr T& operator[](uint8_t i) { return const_cast<T&>(std::as_const(*this)[i]); }

	T width  = T{0};
	T height = T{0};
};

} // namespace aether