#pragma once

#include <aether/mat3.hh>
#include <aether/rect.hh>
#include <aether/rgba.hh>

namespace aether::_renderer_impl {

struct draw_guard_ final {
	draw_guard_();
	~draw_guard_();
};

void setup_2d_();

} // namespace aether::_renderer_impl

struct Texture;

namespace aether::renderer {
void draw_texture(Texture const& texture, rect<float> src, mat3 const& transform, rgba color);
} // namespace aether::renderer