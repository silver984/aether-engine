#pragma once

#include <aether/scene_scheduler.hh>

namespace aether {

struct context final {
	aether::scene_scheduler* scene_scheduler;
};

} // namespace aether