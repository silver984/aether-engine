#pragma once

#include "../scene_scheduler.hh"
#include "aether/window.hh"
#include "application.hh"

namespace aether {

class engine final : public application {
protected:
	void close_() override;

	void pre_run_() override;

	void update_(float dt) override { scene_scheduler_.update_scene_(dt); }
	void draw_() override { scene_scheduler_.draw_scene_(); }

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
};

} // namespace aether