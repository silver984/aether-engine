#pragma once

#include <cstdint>

namespace aether::_ref_impl {

struct block_ {
	virtual void release_ptr()  = 0;
	virtual void release_self() = 0;
};

struct counted_block_ : block_ {
	uint32_t strong_count;
	uint32_t weak_count;
};

template <typename T_>
struct shared_block_ final : counted_block_ {
	explicit shared_block_(T_* p)
	        : ptr(p) {}

	void release_ptr() override {
		delete ptr;
		ptr = nullptr;
	}

	void release_self() override { delete this; }

	T_* ptr;
};

template <typename T_>
struct unique_block_ final : block_ {
	explicit unique_block_(T_* p)
	        : ptr(p) {}

	void release_ptr() override {
		delete ptr;
		ptr = nullptr;
	}

	void release_self() override { delete this; }

	T_* ptr;
};

} // namespace aether::_ref_impl