#pragma once

#include "../strong_ref.hh"
#include "../unique_ref.hh"

namespace aether::_ref_impl {

struct factory_ final {
	factory_() = delete;

	template <typename T, typename... Args>
	[[nodiscard]] static strong_ref<T> strong(Args&&... args) {
		return strong_ref<T>(new T(std::forward<Args>(args)...));
	}

	template <typename T, typename... Args>
	[[nodiscard]] static unique_ref<T> unique(Args&&... args) {
		return unique_ref<T>(new T(std::forward<Args>(args)...));
	}

	template <typename T, typename U>
	        requires std::is_base_of_v<U, T>
	[[nodiscard]] static strong_ref<T> dynamic_strong_cast(strong_ref<U> const& a) {
		T* ptr = dynamic_cast<T*>(a.ptr_);
		if (!ptr) {
			return nullptr;
		}
		strong_ref<T> out;
		out.ptr_   = ptr;
		out.block_ = a.block_;
		out.increment_strong_count_();
		return out;
	}
};

} // namespace aether::_ref_impl