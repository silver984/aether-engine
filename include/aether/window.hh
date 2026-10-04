#pragma once

#include <aether/operable_enum_class.hh>
#include <aether/size.hh>

#include <cstdint>
#include <string_view>

namespace aether::window {

enum class flags : uint8_t {
	none        = 0,
	resizable   = 1 << 0,
	undecorated = 1 << 1,
};

struct configuration final {
	std::string_view title                         = "unnamed";
	size<uint32_t> bounds                          = {360};
	uint32_t fps                                   = 60;
	util::operable_enum_class<window::flags> flags = flags::none;
};

[[nodiscard]] size<uint32_t> bounds();
[[nodiscard]] uint32_t fps();

} // namespace aether::window

namespace aether::_window_impl {

bool create_(window::configuration const& cfg);
void close_();
[[nodiscard]] bool should_close_();
[[nodiscard]] bool is_minimized_();

} // namespace aether::_window_impl
