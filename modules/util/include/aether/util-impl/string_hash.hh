#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace aether::_util_impl {

class string_hash_ final {
private:
	using hash_ = std::hash<std::string_view>;

public:
	using is_transparent = void;
	size_t operator()(std::string_view str) const { return hash_{}(str); }
	size_t operator()(std::string const& str) const { return hash_{}(str); }
	size_t operator()(char const* str) const { return hash_{}(str); }
};

} // namespace aether::_util_impl