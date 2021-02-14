#pragma once
#include <vector>
#include <cassert>

namespace pf {
	template<typename T>
	struct Slice {
		using iterator = const T*;
		using const_iterator = iterator;

		~Slice() = default;
		Slice(const Slice&) noexcept = default;
		Slice& operator=(const Slice&) noexcept = default;
		
		Slice() noexcept
			: dataBegin(nullptr)
			, dataEnd(nullptr)
		{}
		Slice(const T* first, const T* last) noexcept
			: dataBegin(first)
			, dataEnd(last)
		{}
		Slice(const std::vector<T>& arr) noexcept
			: dataBegin(arr.data())
			, dataEnd(arr.data() + arr.size())
		{}
		Slice(const std::vector<T>& arr, std::size_t s, std::size_t e) noexcept
			: dataBegin(arr.data() + s)
			, dataEnd(arr.data() + (e-s))
		{
			assert(s <= e);
			assert(e <= arr.size());
		}

		bool empty() const noexcept {
			return dataBegin == dataEnd();
		}

		const T& front() const {
			assert(!empty());
			return *dataBegin;
		}
		const T& back() const {
			assert(!empty());
			return *(dataEnd - 1);
		}

		const T& operator[](std::size_t index) const {
			assert(dataBegin + index < dataEnd);
			return *(dataBegin + index);
		}

		const T* data() const noexcept {
			return dataBegin;
		}

		std::size_t size() const noexcept {
			return dataEnd - dataBegin;
		}

		const_iterator begin() const noexcept {
			return dataBegin;
		}
		const_iterator end() const noexcept {
			return dataEnd;
		}

		const_iterator cbegin() const noexcept {
			return dataBegin;
		}
		const_iterator cend() const noexcept {
			return dataEnd;
		}

		const_iterator rbegin() const noexcept {
			return dataEnd - 1;
		}
		const_iterator rend() const noexcept {
			return dataBegin - 1;
		}

		const_iterator crbegin() const noexcept {
			return dataEnd - 1;
		}
		const_iterator crend() const noexcept {
			return dataBegin - 1;
		}

		bool operator==(const Slice& other) const noexcept {
			return dataBegin == other.dataBegin && dataEnd == other.dataEnd;
		}
		bool operator!=(const Slice& other) const noexcept {
			return dataBegin != other.dataBegin || dataEnd != other.dataEnd;
		}
	private:
		const T* dataBegin, * dataEnd;
	};
};