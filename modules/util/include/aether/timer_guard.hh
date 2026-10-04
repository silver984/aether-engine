#pragma once

#include <aether/log.hh>

#include <chrono>

namespace aether::util {

template <_log_impl::loggable_level_ T>
class timer_guard final {
public:
	explicit timer_guard(_log_impl::format_string_ const& str) {
		log<T>(str);
		start_timepoint_ = std::chrono::steady_clock::now();
	}

	~timer_guard() {
		auto const now = std::chrono::steady_clock::now();
		auto const ms  = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_timepoint_);
		long long dur  = ms.count();
		log<T>({"Timer took {}ms", dur});
	}

private:
	std::chrono::steady_clock::time_point start_timepoint_;
};

} // namespace aether::util