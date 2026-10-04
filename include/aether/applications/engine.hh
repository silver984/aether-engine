#pragma once

#include "application.hh"

#include <aether/scene_scheduler.hh>

namespace aether {

class engine final : public application {
protected:
	void close_() override;

	void pre_run_() override;

	void update_(float dt) override;
	void draw_() override;

	[[nodiscard]] window::configuration window_configuration_() const override {
		return {
		        .title  = "Aether Engine",
		        .bounds = {1280, 720},
		        .fps    = 144,
		        .flags  = window::flags::resizable,
		};
	}

private:
	scene_scheduler scene_scheduler_;
	float time_elapsed_     = 0.f;
	uint32_t frame_count_   = 0;
	uint32_t evaluated_fps_ = 0;
};

} // namespace aether