#pragma once
#include <cinttypes>

namespace pf {
	struct half {
		half(const half&) noexcept = default;
		half& operator=(const half&) noexcept = default;
		~half() = default;

		half(float val = 0.f) noexcept;
		half(double val = 0.0) noexcept;
		half(long double val = static_cast<long double>(0)) noexcept;

		operator float() const noexcept;
		operator double() const noexcept;
		explicit operator long double() const noexcept;

		half& operator=(float val) noexcept;
		half& operator=(double val) noexcept;
		half& operator=(long double val) noexcept;

		bool operator==(half val) const noexcept;
		bool operator!=(half val) const noexcept;
	private:
		uint16_t data;

		friend float makeFloat(half val);
		friend double makeDouble(half val);
		friend uint16_t makeHalf(float val);
		friend uint16_t makeHalf(double val);
	};
};