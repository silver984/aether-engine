#pragma once

#include "numeric_type.hh"

namespace aether::_math_impl {

struct addition_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	[[nodiscard]] constexpr auto operator()(Lhs_ lhs, Rhs_ rhs) const {
		return lhs + rhs;
	}
};

struct addition_assign_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	constexpr Lhs_& operator()(Lhs_& lhs, Rhs_ rhs) const {
		return lhs += rhs;
	}
};

struct subtraction_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	[[nodiscard]] constexpr auto operator()(Lhs_ lhs, Rhs_ rhs) const {
		return lhs - rhs;
	}
};

struct subtraction_assign_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	constexpr Lhs_& operator()(Lhs_& lhs, Rhs_ rhs) const {
		return lhs -= rhs;
	}
};

struct multiplication_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	[[nodiscard]] constexpr auto operator()(Lhs_ lhs, Rhs_ rhs) const {
		return lhs * rhs;
	}
};

struct multiplication_assign_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	constexpr Lhs_& operator()(Lhs_& lhs, Rhs_ rhs) const {
		return lhs *= rhs;
	}
};

struct division_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	[[nodiscard]] constexpr auto operator()(Lhs_ lhs, Rhs_ rhs) const {
		return lhs / rhs;
	}
};

struct division_assign_ final {
	template <numeric_type_ Lhs_, numeric_type_ Rhs_>
	constexpr Lhs_& operator()(Lhs_& lhs, Rhs_ rhs) const {
		return lhs /= rhs;
	}
};

} // namespace aether::_math_impl