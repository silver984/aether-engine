#pragma once

#include "ref-impl/block.hh"

#include <concepts>
#include <cstddef>
#include <new>
#include <utility>

namespace aether::_ref_impl {
struct factory_;
}

namespace aether {

template <typename T>
class unique_ref final {
	friend class _ref_impl::factory_;

	template <typename>
	friend class unique_ref;

public:
	unique_ref() = default;
	unique_ref(std::nullptr_t) {}
	unique_ref(unique_ref const&) = delete;

	unique_ref(unique_ref&& ref)
	        : block_(std::exchange(ref.block_, nullptr)) {}

	template <std::derived_from<T> U>
	unique_ref(unique_ref<U>&& ref)
	        : block_(std::exchange(ref.block_, nullptr)) {}

	~unique_ref() { release(); }

	void release() {
		if (!block_) {
			return;
		}
		block_->release_ptr();
		block_->release_self();
		block_ = nullptr;
	}

	[[nodiscard]] T* get() const { return block_ ? static_cast<_ref_impl::unique_block_<T>*>(block_)->ptr : nullptr; }
	[[nodiscard]] T* operator->() const { return get(); }
	[[nodiscard]] T& operator*() const { return *get(); }

	explicit operator bool() const { return get() != nullptr; }

	unique_ref& operator=(unique_ref const&) = delete;

	unique_ref& operator=(unique_ref&& ref) {
		if (this != &ref) {
			return move_(ref);
		}
		return *this;
	}

	template <std::derived_from<T> U>
	unique_ref& operator=(unique_ref<U>&& ref) {
		return move_(ref);
	}

	[[nodiscard]] bool operator==(std::nullptr_t) const { return get() == nullptr; }
	[[nodiscard]] bool operator!=(std::nullptr_t) const { return !(*this == nullptr); }

	template <std::derived_from<T> U>
	[[nodiscard]] bool operator==(unique_ref<U> const& ref) const {
		return get() == ref.get();
	}

	template <std::derived_from<T> U>
	[[nodiscard]] bool operator!=(unique_ref<U> const& ref) const {
		return !(*this == ref);
	}

private:
	explicit unique_ref(T* ptr) {
		if (!ptr) {
			return;
		}

		block_ = new (std::nothrow) _ref_impl::unique_block_(ptr);

		if (!block_) {
			delete ptr;
			return;
		}
	}

	template <std::derived_from<T> U_>
	unique_ref& move_(unique_ref<U_>& ref) {
		release();
		block_ = std::exchange(ref.block_, nullptr);
		return *this;
	}

	_ref_impl::block_* block_ = nullptr;
};

} // namespace aether