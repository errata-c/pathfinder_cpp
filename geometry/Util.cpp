#include "Util.hpp"
#include <cmath>
#include <algorithm>

namespace pf {
	bool approx_eq(float a, float b) noexcept {
		return std::abs(a - b) <= Eps;
	}
	float lerp(float a, float b, float t) noexcept {
		return (b - a) * t + a;
	}
	float clamp(float val, float low, float high) noexcept {
		return std::max(low, std::min(val, high));
	}
	int32_t alignup_i32(int32_t a, int32_t b) noexcept {
		return (a + b - 1) / b;
	}
}