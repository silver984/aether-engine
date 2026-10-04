#include <aether/log.hh>
#include <aether/math.hh>
#include <aether/window.hh>

#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

namespace aether::_window_impl {

size<uint32_t> bounds_;
uint32_t fps_ = 0;

bool create_(window::configuration const& cfg) {
	unsigned int flags = FLAG_WINDOW_ALWAYS_RUN;

	bool const undecorated = (cfg.flags & window::flags::undecorated) != window::flags::none;
	bool const resizable   = (cfg.flags & window::flags::resizable) != window::flags::none && !undecorated;

	if (undecorated) {
		flags |= FLAG_WINDOW_UNDECORATED;
	}

	if (resizable) {
		flags |= FLAG_WINDOW_RESIZABLE;
	}

	SetConfigFlags(flags);
	InitWindow(cfg.bounds.width, cfg.bounds.height, cfg.title.data());

	if (!IsWindowReady()) {
		return false;
	}

	SetTargetFPS(0);
	SetExitKey(KEY_NULL);
	SetWindowMinSize(cfg.bounds.width / 2, cfg.bounds.height / 2);

	rlImGuiSetup(true);
	ImGuiIO& io    = ImGui::GetIO();
	io.IniFilename = nullptr;

	bounds_ = cfg.bounds;
	fps_    = cfg.fps;

	return true;
}

void close_() {
	rlImGuiShutdown();
	CloseWindow();
}

bool should_close_() { return WindowShouldClose(); }
bool is_minimized_() { return IsWindowMinimized(); }

} // namespace aether::_window_impl

namespace aether::window {

size<uint32_t> bounds() { return _window_impl::bounds_; }
uint32_t fps() { return _window_impl::fps_; }

} // namespace aether::window