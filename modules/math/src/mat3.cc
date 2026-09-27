#include <aether/mat3.hh>
#include <aether/math.hh>

#include <cmath>

namespace aether {

mat3 mat3::rotation(float rad) {
	float const c = std::cos(rad);
	float const s = std::sin(rad);
	mat3 out;
	out.m_[0][0] = c;
	out.m_[0][1] = -s;
	out.m_[1][0] = s;
	out.m_[1][1] = c;
	return out;
}

mat3 mat3::skew(vec2<float> rad) {
	vec2<float> const t = tan(rad);
	mat3 out;
	out.m_[0][1] = t.x;
	out.m_[1][0] = t.y;
	return out;
}

} // namespace aether