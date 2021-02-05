#pragma once
#include <cinttypes>

namespace pf {
	static constexpr float Eps = 0.001f;

	bool approx_eq(float a, float b) noexcept;
	float lerp(float a, float b, float t) noexcept;
	float clamp(float val, float low, float high) noexcept;
	int32_t alignup_i32(int32_t a, int32_t b) noexcept;
};