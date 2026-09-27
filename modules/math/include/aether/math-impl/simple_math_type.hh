#pragma once

#include "numeric_type.hh"
#include "operations.hh"

#include <concepts>
#include <cstdint>
#include <type_traits>

namespace aether::_math_impl {

template <typename T_>
concept indexable_math_type_ = numeric_type_<typename T_::_value_type> && requires(T_ v, T_ const cv, uint8_t i) {
	{ T_::capacity() } -> std::same_as<uint8_t>;
	{ v[i] } -> std::same_as<typename T_::_value_type&>;
	{ cv[i] } -> std::same_as<typename T_::_value_type const&>;
};

template <typename T_, uint8_t N_>
concept matching_capacity_math_type_ = indexable_math_type_<T_> && requires { requires T_::capacity() == N_; };

template <template <numeric_type_> typename T_, numeric_type_ V_, uint8_t N_>
class simple_math_type_ {
public:
	using _value_type = V_;

private:
	using return_type_ = T_<_value_type>;

public:
	[[nodiscard]] static constexpr uint8_t capacity() { return N_; }

	[[nodiscard]] constexpr return_type_ operator+() const { return self_(); }

	[[nodiscard]] constexpr return_type_ operator-() const {
		return_type_ out;
		for (uint8_t i = 0; i < capacity(); ++i) {
			out[i] = -(self_()[i]);
		}
		return out;
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] constexpr auto operator+(Other const& other) const {
		return evaluate_<addition_>(other);
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] constexpr auto operator-(Other const& other) const {
		return evaluate_<subtraction_>(other);
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] constexpr auto operator*(Other const& other) const {
		return evaluate_<multiplication_>(other);
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] constexpr auto operator/(Other const& other) const {
		return evaluate_<division_>(other);
	}

	template <numeric_type_ Num>
	[[nodiscard]] constexpr auto operator+(Num const& val) const {
		return evaluate_<addition_>(val);
	}

	template <numeric_type_ Num>
	[[nodiscard]] constexpr auto operator-(Num const& val) const {
		return evaluate_<subtraction_>(val);
	}

	template <numeric_type_ Num>
	[[nodiscard]] constexpr auto operator*(Num const& val) const {
		return evaluate_<multiplication_>(val);
	}

	template <numeric_type_ Num>
	[[nodiscard]] constexpr auto operator/(Num const& val) const {
		return evaluate_<division_>(val);
	}

	template <matching_capacity_math_type_<N_> Other>
	constexpr return_type_& operator+=(Other const& other) {
		return perform_<addition_assign_>(other);
	}

	template <matching_capacity_math_type_<N_> Other>
	constexpr return_type_& operator-=(Other const& other) {
		return perform_<subtraction_assign_>(other);
	}

	template <matching_capacity_math_type_<N_> Other>
	constexpr return_type_& operator*=(Other const& other) {
		return perform_<multiplication_assign_>(other);
	}

	template <matching_capacity_math_type_<N_> Other>
	constexpr return_type_& operator/=(Other const& other) {
		return perform_<division_assign_>(other);
	}

	template <numeric_type_ Num>
	constexpr return_type_& operator+=(Num const& val) {
		return perform_<addition_assign_>(val);
	}

	template <numeric_type_ Num>
	constexpr return_type_& operator-=(Num const& val) {
		return perform_<subtraction_assign_>(val);
	}

	template <numeric_type_ Num>
	constexpr return_type_& operator*=(Num const& val) {
		return perform_<multiplication_assign_>(val);
	}

	template <numeric_type_ Num>
	constexpr return_type_& operator/=(Num const& val) {
		return perform_<division_assign_>(val);
	}

	constexpr return_type_& operator++() {
		for (uint8_t i = 0; i < capacity(); ++i) {
			++self_()[i];
		}
		return self_();
	}

	constexpr return_type_ operator++(int) {
		return_type_ out(self_());
		++self_();
		return out;
	}

	constexpr return_type_& operator--() {
		for (uint8_t i = 0; i < capacity(); ++i) {
			--self_()[i];
		}
		return self_();
	}

	constexpr return_type_ operator--(int) {
		return_type_ out(self_());
		--self_();
		return out;
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] constexpr bool operator==(Other const& other) const {
		for (uint8_t i = 0; i < capacity(); ++i) {
			if (self_()[i] != other[i]) {
				return false;
			}
		}
		return true;
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] constexpr bool operator!=(Other const& other) const {
		return !(self_() == other);
	}

	template <matching_capacity_math_type_<N_> Other>
	[[nodiscard]] explicit constexpr operator Other() const {
		Other out;
		for (uint8_t i = 0; i < capacity(); ++i) {
			out[i] = static_cast<Other::_value_type>(self_()[i]);
		}
		return out;
	}

private:
	template <numeric_type_ Num_>
	using common_value_type_ = std::common_type_t<_value_type, Num_>;

	template <matching_capacity_math_type_<N_> Other_>
	using common_math_value_type_ = common_value_type_<typename Other_::_value_type>;

	template <matching_capacity_math_type_<N_> Other_>
	using common_indexable_math_type_ = T_<common_math_value_type_<Other_>>;

	[[nodiscard]] constexpr return_type_& self_() { return static_cast<return_type_&>(*this); }
	[[nodiscard]] constexpr return_type_ const& self_() const { return static_cast<return_type_ const&>(*this); }

	template <typename Operation_, matching_capacity_math_type_<N_> Other_>
	[[nodiscard]] constexpr auto evaluate_(Other_ const& other) const {
		common_indexable_math_type_<Other_> out;
		Operation_ op{};
		for (uint8_t i = 0; i < capacity(); ++i) {
			out[i] = op(static_cast<common_math_value_type_<Other_>>(self_()[i]),
			            static_cast<common_math_value_type_<Other_>>(other[i]));
		}
		return out;
	}

	template <typename Operation_, numeric_type_ Num_>
	[[nodiscard]] constexpr auto evaluate_(Num_ const& val) const {
		using common = common_value_type_<Num_>;
		T_<common> out;
		Operation_ op{};
		for (uint8_t i = 0; i < capacity(); ++i) {
			out[i] = op(static_cast<common>(self_()[i]), static_cast<common>(val));
		}
		return out;
	}

	template <typename Operation_, matching_capacity_math_type_<N_> Other_>
	constexpr return_type_& perform_(Other_ const& other) {
		Operation_ op{};
		for (uint8_t i = 0; i < capacity(); ++i) {
			op(self_()[i], static_cast<_value_type>(other[i]));
		}
		return self_();
	}

	template <typename Operation_, numeric_type_ Num_>
	constexpr return_type_& perform_(Num_ const& val) {
		Operation_ op{};
		for (uint8_t i = 0; i < capacity(); ++i) {
			op(self_()[i], static_cast<_value_type>(val));
		}
		return self_();
	}
};

} // namespace aether::_math_impl