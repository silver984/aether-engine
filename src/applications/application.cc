#include <aether/applications/application.hh>
#include <aether/log.hh>
#include <aether/math.hh>
#include <aether/renderer.hh>
#include <aether/timer_guard.hh>
#include <aether/window.hh>

#include <raylib.h>

#include <chrono>
#include <thread>

namespace aether {

void application::run() {
	if (!initialization_result_) {
		log<error>({"Can't run uninitialized application"});
		return;
	}

	auto last_frametime = std::chrono::steady_clock::now();
	auto next_frametime = last_frametime;

	bool window_was_minimized = false;

	pre_run_();

	while (!_window_impl::should_close_()) {
		auto const now = std::chrono::steady_clock::now();
		float const dt = std::chrono::duration<float>(now - last_frametime).count();
		last_frametime = now;

		bool const window_currently_minimized = _window_impl::is_minimized_();

		if (window_currently_minimized) {
			if (!window_was_minimized) {
				window_was_minimized = true;
				on_window_minimized_();
			}
		} else {
			if (window_was_minimized) {
				window_was_minimized = false;
				on_window_restored_();
			}
			update_(dt);
		}

		{
			_renderer_impl::draw_guard_ const draw_guard;
			if (!window_currently_minimized) {
				draw_();
			}
		}

		if (window_currently_minimized) {
			next_frametime = std::chrono::steady_clock::now();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		std::chrono::duration<float> const ideal_dt(1.f / window::fps());
		next_frametime += std::chrono::duration_cast<std::chrono::steady_clock::duration>(ideal_dt);
		std::this_thread::sleep_until(next_frametime);
	}

	post_run_();
	shutdown_();
}

bool application::init_() {
	window::configuration cfg = window_configuration_();
	cfg.bounds                = max(cfg.bounds, {360});
	cfg.fps                   = std::max(cfg.fps, static_cast<uint32_t>(15));

	if (!_window_impl::create_(cfg)) {
		return false;
	}

	_renderer_impl::setup_2d_();
	return true;
}

void application::shutdown_() {
	util::timer_guard<info> const timer({"Shutting down"});
	close_();
	_window_impl::close_();
}

} // namespace aether