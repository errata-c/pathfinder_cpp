#include "half.hpp"

namespace pf {
	union ConvertFloat {
		float val;
		uint32_t ival;
	};
	union ConvertDouble {
		double val;
		uint64_t ival;
	};

	static float makeFloat(half val) {
		ConvertFloat convert;
		uint32_t exponent = ((val.data & 0x7C00) + 0x1C000);

		convert.ival =
			((val.data & 0x8000) << 16) |
			(exponent << 13) |
			((val.data & 0x3FF) << 13);

		return convert.val;
	}
	static double makeDouble(half val) {
		ConvertDouble convert;
		uint64_t exponent = (val.data & 0x7C00ull) + 0xFC000ull;

		convert.ival =
			(static_cast<uint64_t>(val.data & 0x8000) << 48) |
			(exponent << 42) |
			(static_cast<uint64_t>(val.data & 0x03FF) << 42);
		return convert.val;
	}
	static uint16_t makeHalf(float val) {
		ConvertFloat convert;
		convert.val = val;
		uint16_t res =
			((convert.ival & 0xC000'0000) >> 16) |
			((convert.ival & 0x0780'0000) >> 13) |
			((convert.ival & 0x007F'C000) >> 13);
		return res;
	}
	static uint16_t makeHalf(double val) {
		ConvertDouble convert;
		convert.val = val;
		uint16_t res =
			((convert.ival & 0xC000'0000'0000'0000ull) >> 48) |
			((convert.ival & 0x0780'0000'0000'0000ull) >> 42) |
			((convert.ival >> 42) & 0x03FFull);
		return res;
	}

	half::half(float val) noexcept
		: data(makeHalf(val))
	{}
	half::half(double val) noexcept
		: data(makeHalf(val))
	{}
	half::half(long double val) noexcept
		: data(makeHalf(static_cast<double>(val)))
	{}

	half::operator float() const noexcept {
		return makeFloat(*this);
	}
	half::operator double() const noexcept {
		return makeDouble(*this);
	}
	half::operator long double() const noexcept {
		return static_cast<long double>(makeDouble(*this));
	}

	half& half::operator=(float val) noexcept {
		data = makeHalf(val);
		return *this;
	}
	half& half::operator=(double val) noexcept {
		data = makeHalf(val);
		return *this;
	}
	half& half::operator=(long double val) noexcept {
		data = makeHalf(static_cast<double>(val));
		return *this;
	}

	bool half::operator==(half val) const noexcept {
		return data == val.data;
	}
	bool half::operator!=(half val) const noexcept {
		return data != val.data;
	}
};