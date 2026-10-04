#pragma once

#include <type_traits>

namespace aether {

template <typename T>
        requires std::is_scoped_enum_v<T>
class operable_enum_class final {
private:
	using underlying_ = std::underlying_type_t<T>;

public:
	constexpr operable_enum_class(T val)
	        : val_(val) {}

	constexpr operator T() const { return val_; }

	constexpr operable_enum_class& operator=(T val) {
		val_ = val;
		return *this;
	}

	constexpr operable_enum_class operator|(operable_enum_class other) const {
		return static_cast<T>(static_cast<underlying_>(val_) | static_cast<underlying_>(other.val_));
	}

	constexpr operable_enum_class operator&(operable_enum_class other) const {
		return static_cast<T>(static_cast<underlying_>(val_) & static_cast<underlying_>(other.val_));
	}

	constexpr operable_enum_class operator^(operable_enum_class other) const {
		return static_cast<T>(static_cast<underlying_>(val_) ^ static_cast<underlying_>(other.val_));
	}

	constexpr operable_enum_class operator~() const { return static_cast<T>(~static_cast<underlying_>(val_)); }
	constexpr operable_enum_class& operator|=(operable_enum_class other) { return *this = *this | other; }
	constexpr operable_enum_class& operator&=(operable_enum_class other) { return *this = *this & other; }
	constexpr operable_enum_class& operator^=(operable_enum_class other) { return *this = *this ^ other; }

private:
	T val_;
};

} // namespace aether