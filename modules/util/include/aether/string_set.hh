#pragma once

#include "util-impl/string_hash.hh"

#include <unordered_set>

namespace aether::util {
using string_set = std::unordered_set<std::string, _util_impl::string_hash_, std::equal_to<>>;
}