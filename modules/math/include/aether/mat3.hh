#pragma once

#include "vec2.hh"

namespace aether {

class mat3 final {
public:
	mat3();

	[[nodiscard]] static mat3 rotation(float rad);
	[[nodiscard]] static mat3 skew(vec2<float> rad);
	[[nodiscard]] static mat3 identity() { return {}; }
	[[nodiscard]] static mat3 translation(vec2<float> t);
	[[nodiscard]] static mat3 scale(vec2<float> s);

	[[nodiscard]] vec2<float> transform_point(vec2<float> p) const;
	[[nodiscard]] vec2<float> translation() const;

	[[nodiscard]] mat3 operator*(mat3 const& other) const;

	[[nodiscard]] float* operator[](uint8_t i) { return m_[i]; }
	[[nodiscard]] float const* operator[](uint8_t i) const { return const_cast<mat3&>(*this)[i]; }

private:
	float m_[3][3];
};

} // namespace aether