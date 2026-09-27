#pragma once

#include "ref-impl/block.hh"
#include "strong_ref.hh"
#include <utility>

namespace aether {

template <typename T>
class weak_ref final {
	template <typename>
	friend class self_ref;

private:
	using block_type_ = _ref_impl::shared_block_<T>;

public:
	weak_ref() = default;
	weak_ref(std::nullptr_t) {}

	weak_ref(weak_ref const& ref)
	        : ptr_(ref.ptr_)
	        , block_(ref.block_) {
		increment_weak_count_();
	}

	weak_ref(weak_ref&& ref) {
		ptr_   = std::exchange(ref.ptr_, nullptr);
		block_ = std::exchange(ref.block_, nullptr);
	}

	weak_ref(strong_ref<T> const& ref)
	        : ptr_(ref.ptr_)
	        , block_(ref.block_) {
		increment_weak_count_();
	}

	template <std::derived_from<T> U>
	weak_ref(strong_ref<U> const& ref)
	        : ptr_(static_cast<T*>(ref.ptr_))
	        , block_(ref.block_) {
		increment_weak_count_();
	}

	~weak_ref() { detach(); }

	void detach() {
		if (!block_) {
			return;
		}

		_ref_impl::counted_block_* old_block = std::exchange(block_, nullptr);
		ptr_                                 = nullptr;

		if (--old_block->weak_count == 0 && old_block->strong_count == 0) {
			old_block->release_self();
		}
	}

	[[nodiscard]] strong_ref<T> construct() const {
		if (is_expired()) {
			return nullptr;
		}
		strong_ref<T> out;
		out.ptr_   = ptr_;
		out.block_ = block_;
		out.increment_strong_count_();
		return out;
	}

	[[nodiscard]] bool is_expired() const { return block_ == nullptr || block_->strong_count == 0; }

	weak_ref& operator=(weak_ref const& ref) {
		if (this == &ref) {
			return *this;
		}
		detach();
		return copy_(ref.ptr_, ref.block_);
	}

	weak_ref& operator=(weak_ref&& ref) {
		if (this == &ref) {
			return *this;
		}
		detach();
		ptr_   = std::exchange(ref.ptr_, nullptr);
		block_ = std::exchange(ref.block_, nullptr);
		return *this;
	}

	weak_ref& operator=(strong_ref<T> const& ref) {
		detach();
		return copy_(ref.ptr_, ref.block_);
	}

	template <std::derived_from<T> U>
	weak_ref& operator=(strong_ref<U> const& ref) {
		detach();
		return copy_(static_cast<T*>(ref.ptr_), ref.block_);
	}

private:
	void increment_weak_count_() {
		if (block_) {
			++block_->weak_count;
		}
	}

	weak_ref& copy_(T* p, _ref_impl::counted_block_* b) {
		ptr_   = p;
		block_ = b;
		increment_weak_count_();
		return *this;
	}

	T* ptr_                           = nullptr;
	_ref_impl::counted_block_* block_ = nullptr;
};

} // namespace aether