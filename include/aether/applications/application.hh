#pragma once

#include "../window.hh"

#include <aether/size.hh>

namespace aether {

class application {
public:
	application()                              = default;
	application(application&&)                 = delete;
	application(application const&)            = delete;
	application& operator=(application&&)      = delete;
	application& operator=(application const&) = delete;

	bool init() {
		if (initialization_attempted_) {
			return initialization_result_;
		}
		initialization_result_    = init_();
		initialization_attempted_ = true;
		return initialization_result_;
	}

	void run();

protected:
	virtual bool init_();
	virtual void close_() {}

	virtual void pre_run_() {}
	virtual void post_run_() {}

	virtual void update_(float dt) {}
	virtual void draw_() {}

	virtual void on_window_minimized_() {}
	virtual void on_window_restored_() {}

	[[nodiscard]] virtual window::configuration window_configuration_() const { return {}; }

private:
	void shutdown_();

	bool initialization_attempted_ = false;
	bool initialization_result_    = false;
};

} // namespace aether