#pragma once

#include "vec2.hh"

namespace aether {

class mat3 final {
public:
	constexpr mat3() = default;

	[[nodiscard]] constexpr static mat3 identity() { return {}; }

	[[nodiscard]] static mat3 rotation(float rad);
	[[nodiscard]] static mat3 skew(vec2<float> rad);

	[[nodiscard]] constexpr static mat3 translation(vec2<float> t) {
		mat3 out;
		out.m_[0][2] = t.x;
		out.m_[1][2] = t.y;
		return out;
	}

	[[nodiscard]] constexpr static mat3 scale(vec2<float> s) {
		mat3 out;
		out.m_[0][0] = s.x;
		out.m_[1][1] = s.y;
		return out;
	}

	[[nodiscard]] constexpr vec2<float> transform_point(vec2<float> p) const {
		return {
		        p.x * m_[0][0] + p.y * m_[0][1] + m_[0][2],
		        p.x * m_[1][0] + p.y * m_[1][1] + m_[1][2],
		};
	}

	[[nodiscard]] constexpr vec2<float> translation() const {
		return {
		        m_[0][2],
		        m_[1][2],
		};
	}

	[[nodiscard]] constexpr mat3 operator*(mat3 const& other) const {
		mat3 out;
		for (uint8_t row = 0; row < 3; ++row) {
			for (uint8_t col = 0; col < 3; ++col) {
				out.m_[row][col] =
				        m_[row][0] * other.m_[0][col] + m_[row][1] * other.m_[1][col] + m_[row][2] * other.m_[2][col];
			}
		}
		return out;
	}

	[[nodiscard]] constexpr float* operator[](uint8_t i) { return m_[i]; }
	[[nodiscard]] constexpr float const* operator[](uint8_t i) const { return m_[i]; }

private:
	float m_[3][3] = {
	        {1.f, 0.f, 0.f},
	        {0.f, 1.f, 0.f},
	        {0.f, 0.f, 1.f},
	};
};

} // namespace aether