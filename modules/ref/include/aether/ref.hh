#pragma once

#include "ref-impl/factory.hh"
#include "self_referenceable.hh"
#include "strong_ref.hh"
#include "unique_ref.hh"
#include "weak_ref.hh"

#include <type_traits>

namespace aether {

template <typename T, typename... Args>
[[nodiscard]] strong_ref<T> strong(Args&&... args) {
	return _ref_impl::factory_::strong<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
[[nodiscard]] unique_ref<T> unique(Args&&... args) {
	return _ref_impl::factory_::unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename U>
        requires std::is_base_of_v<U, T>
[[nodiscard]] strong_ref<T> dynamic_strong_cast(strong_ref<U> const& a) {
	return _ref_impl::factory_::dynamic_strong_cast<T>(a);
}

} // namespace aether