#pragma once

#include "aether/ref-impl/block.hh"
#include "ref-impl/block.hh"

#include <concepts>
#include <cstddef>
#include <new>
#include <utility>

namespace aether::_ref_impl {
template <typename T_>
concept self_referenceable_ = requires { typename T_::_is_self_referenceable; };
struct factory_;
} // namespace aether::_ref_impl

namespace aether {

template <typename T>
class strong_ref final {
	friend class _ref_impl::factory_;

	template <typename>
	friend class strong_ref;

	template <typename>
	friend class weak_ref;

public:
	strong_ref() = default;
	strong_ref(std::nullptr_t) {}

	strong_ref(strong_ref const& ref)
	        : ptr_(ref.ptr_)
	        , block_(ref.block_) {
		increment_strong_count_();
	}

	template <std::derived_from<T> U>
	strong_ref(strong_ref<U> const& ref)
	        : ptr_(static_cast<T*>(ref.ptr_))
	        , block_(ref.block_) {
		increment_strong_count_();
	}

	strong_ref(strong_ref&& ref)
	        : ptr_(std::exchange(ref.ptr_, nullptr))
	        , block_(std::exchange(ref.block_, nullptr)) {}

	template <std::derived_from<T> U>
	strong_ref(strong_ref<U>&& ref)
	        : ptr_(static_cast<T*>(std::exchange(ref.ptr_, nullptr)))
	        , block_(std::exchange(ref.block_, nullptr)) {}

	~strong_ref() { release(); }

	void release() {
		if (!block_) {
			return;
		}

		_ref_impl::counted_block_* old_block = std::exchange(block_, nullptr);
		ptr_                                 = nullptr;

		if (--old_block->strong_count != 0) {
			return;
		}

		old_block->release_ptr();

		if (--old_block->weak_count == 0) {
			old_block->release_self();
		}
	}

	[[nodiscard]] T* get() const { return ptr_; }
	[[nodiscard]] T* operator->() const { return get(); }
	[[nodiscard]] T& operator*() const { return *get(); }

	[[nodiscard]] uint32_t strong_count() const { return block_ ? block_->strong_count : 0; }
	[[nodiscard]] uint32_t weak_count() const { return block_ ? (block_->weak_count - 1) : 0; }

	explicit operator bool() const { return get() != nullptr; }

	strong_ref& operator=(strong_ref const& ref) {
		if (this != &ref) {
			return copy_(ref);
		}
		return *this;
	}

	template <std::derived_from<T> U>
	strong_ref& operator=(strong_ref<U> const& ref) {
		return copy_(ref);
	}

	strong_ref& operator=(strong_ref&& ref) {
		if (this != &ref) {
			return move_(ref);
		}
		return *this;
	}

	template <std::derived_from<T> U>
	strong_ref& operator=(strong_ref<U>&& ref) {
		return move_(ref);
	}

	[[nodiscard]] bool operator==(std::nullptr_t) const { return get() == nullptr; }
	[[nodiscard]] bool operator!=(std::nullptr_t) const { return !(*this == nullptr); }

	template <std::derived_from<T> U>
	[[nodiscard]] bool operator==(strong_ref<U> const& ref) const {
		return get() == ref.get();
	}

	template <std::derived_from<T> U>
	[[nodiscard]] bool operator!=(strong_ref<U> const& ref) const {
		return !(*this == ref);
	}

private:
	explicit strong_ref(T* ptr) {
		if (!ptr) {
			return;
		}

		block_ = new (std::nothrow) _ref_impl::shared_block_(ptr);

		if (!block_) {
			delete ptr;
			return;
		}

		ptr_                 = ptr;
		block_->strong_count = 1;
		block_->weak_count   = 1; // implicit count

		if constexpr (_ref_impl::self_referenceable_<T>) {
			ptr->init_self_reference_(*this);
		}
	}

	void increment_strong_count_() {
		if (block_) {
			++block_->strong_count;
		}
	}

	template <std::derived_from<T> U_>
	strong_ref& copy_(strong_ref<U_> const& ref) {
		release();
		ptr_   = static_cast<T*>(ref.ptr_);
		block_ = ref.block_;
		increment_strong_count_();
		return *this;
	}

	template <std::derived_from<T> U_>
	strong_ref& move_(strong_ref<U_>& ref) {
		release();
		ptr_   = static_cast<T*>(std::exchange(ref.ptr_, nullptr));
		block_ = std::exchange(ref.block_, nullptr);
		return *this;
	}

	T* ptr_                           = nullptr;
	_ref_impl::counted_block_* block_ = nullptr;
};

} // namespace aether