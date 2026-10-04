#pragma once

#include "application.hh"

namespace aether {

class setup final : public application {
protected:
	void draw_() override;

	[[nodiscard]] window::configuration window_configuration_() const override {
		return {
		        .title  = "Aether Setup",
		        .bounds = {640, 480},
		        .flags  = window::flags::undecorated,
		};
	}
};

} // namespace aether