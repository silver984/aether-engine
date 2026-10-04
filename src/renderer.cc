#include <aether/renderer.hh>
#include <aether/size.hh>
#include <aether/vec2.hh>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include <cmath>

namespace aether::_renderer_impl {

draw_guard_::draw_guard_() {
	BeginDrawing();
	ClearBackground(RAYWHITE);
}

draw_guard_::~draw_guard_() { EndDrawing(); }

void setup_2d_() {
	rlDisableBackfaceCulling();
	rlDisableDepthTest();
}

void define_color_vertex_(rgba color) {
	Color const c = {
	        .r = color.r,
	        .g = color.g,
	        .b = color.b,
	        .a = color.a,
	};
	rlColor4ub(c.r, c.g, c.b, c.a);
}

void define_vertex_(vec2<float> position) { rlVertex2f(position.x, position.y); }
void define_texture_coord_(vec2<float> position) { rlTexCoord2f(position.x, position.y); }

struct matrix_guard_ final {
	explicit matrix_guard_(mat3 const& mat) {
		rlPushMatrix();
		Matrix m{};
		m.m0  = mat[0][0];
		m.m4  = mat[0][1];
		m.m12 = mat[0][2];
		m.m1  = mat[1][0];
		m.m5  = mat[1][1];
		m.m13 = mat[1][2];
		m.m10 = 1.f;
		m.m15 = 1.f;
		rlMultMatrixf(MatrixToFloat(m));
	}
	~matrix_guard_() { rlPopMatrix(); }
};

struct texture_guard_ final {
	explicit texture_guard_(unsigned int id) { rlSetTexture(id); }
	~texture_guard_() { rlSetTexture(0); }
};

struct vertex_draw_guard_ final {
	vertex_draw_guard_() { rlBegin(RL_QUADS); }
	~vertex_draw_guard_() { rlEnd(); }
};

} // namespace aether::_renderer_impl

namespace aether::renderer {

void draw_texture(Texture const& texture, rect<float> src, mat3 const& transform, rgba color) {
	if (texture.id < 1) {
		return;
	}

	bool flip_x = false;

	if (src.width < 0.f) {
		flip_x    = true;
		src.width = std::abs(src.width);
	}

	if (src.height < 0) {
		src.y -= src.height;
	}

	size<float> const bounds = {
	        static_cast<float>(texture.width),
	        static_cast<float>(texture.height),
	};

	vec2<float> const src_pos = src.position();

	{
		_renderer_impl::matrix_guard_ const matrix_guard(transform);
		_renderer_impl::texture_guard_ const texture_guard(texture.id);
		_renderer_impl::vertex_draw_guard_ const draw_guard;

		_renderer_impl::define_color_vertex_(color);
		rlNormal3f(0.f, 0.f, 1.f);

		// top left
		{
			vec2<float> coord = src_pos;
			if (flip_x) {
				coord.x += src.width;
			}
			_renderer_impl::define_texture_coord_(coord / bounds);
			_renderer_impl::define_vertex_({});
		}

		// bottom left
		{
			vec2<float> coord = src_pos;
			coord.y += src.height;
			if (flip_x) {
				coord.x += src.width;
			}
			_renderer_impl::define_texture_coord_(coord / bounds);
			_renderer_impl::define_vertex_({0.f, src.height});
		}

		// bottom right
		{
			vec2<float> coord = src_pos + src.bounds();
			if (flip_x) {
				coord.x -= src.width;
			}
			_renderer_impl::define_texture_coord_(coord / bounds);
			_renderer_impl::define_vertex_({src.width, src.height});
		}

		// top right
		{
			vec2<float> coord = src_pos;
			if (!flip_x) {
				coord.x += src.width;
			}
			_renderer_impl::define_texture_coord_(coord / bounds);
			_renderer_impl::define_vertex_({src.width, 0.f});
		}
	}
}

} // namespace aether::renderer