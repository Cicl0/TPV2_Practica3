// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <bitset>
#include <cassert>
#include <cstddef>

template<typename T, std::size_t SIZE>
class ObjectPool {
public:

	// The purpose of this iterator is to be able to traverse with for-each loop and still
	// get a pointer. If we simply return the pointer as an iterator it does not work.
	//
	struct const_ptr_iterator {
		const T *_ptr;

		const_ptr_iterator(const T *ptr) :
				_ptr(ptr) {
		}

		const T* operator*() const {
			return _ptr;
		}

		bool operator==(const const_ptr_iterator &o) const {
			return _ptr == o._ptr;
		}

		bool operator!=(const const_ptr_iterator &o) const {
			return _ptr != o._ptr;
		}
		const_ptr_iterator& operator++() {
			_ptr++;
			return *this;
		}
		const_ptr_iterator operator++(int) {
			auto tmp = *this;
			++(*this);
			return tmp;
		}
	};

	struct iterator {
		T *_ptr;

		iterator(T *ptr) :
				_ptr(ptr) {
		}

		T* operator*() {
			return _ptr;
		}

		bool operator==(const iterator &o) const {
			return _ptr == o._ptr;
		}

		bool operator!=(const iterator &o) const {
			return _ptr != o._ptr;
		}
		iterator& operator++() {
			_ptr++;
			return *this;
		}
		iterator operator++(int) {
			auto tmp = *this;
			++(*this);
			return tmp;
		}
	};

	ObjectPool() :
			_size(SIZE), //
			_pool(), //
			_used(), //
			_lastUsed(SIZE - 1) {
	}

	virtual ~ObjectPool() {
	}

	T* alloc() {
		auto i = (_lastUsed + 1) % _size;
		while (_used[i] && i != _lastUsed)
			i = (i + 1) % _size;
		if (!_used[i]) {
			_used[i] = true;
			return &_pool[i];
		} else {
			return nullptr;
		}
	}

	inline void free(T *p) {
		auto idx = p - _pool;
		assert(idx >= 0 && idx < _size);
		assert(_used[idx]);
		_used[idx] = false;
	}

	inline T& operator[](std::size_t idx) {
		assert(idx >= 0 && idx < _size);
		return _pool[idx];
	}

	inline std::size_t size() const {
		return _size;
	}

	inline iterator begin() {
		return iterator { _pool };
	}

	inline iterator end() {
		return iterator { _pool + _size };
	}

	inline const_ptr_iterator begin() const {
		return const_ptr_iterator { _pool };
	}

	inline const_ptr_iterator end() const {
		return const_ptr_iterator { _pool + _size };
	}

	inline bool is_used(std::size_t idx) const {
		assert(idx >= 0 && idx < _size);
		return _used[idx];
	}

	inline bool is_used(const T *p) const {
		return is_used(p - _pool);
	}

private:
	std::size_t _size;
	T _pool[SIZE];
	std::bitset<SIZE> _used;
	std::size_t _lastUsed;
};

