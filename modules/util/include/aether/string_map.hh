#pragma once

#include "util-impl/string_hash.hh"

#include <unordered_map>

namespace aether::util {
template <typename T>
using string_map = std::unordered_map<std::string, T, _util_impl::string_hash_, std::equal_to<>>;
}