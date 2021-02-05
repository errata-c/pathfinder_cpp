#pragma once
#include <cinttypes>
#include <iosfwd>
#include <array>
#include <string_view>
#include <glm/vec4.hpp>

namespace pf {
	struct ColorMatrix: public std::array<glm::vec4, 5> {
		ColorMatrix(const ColorMatrix&) noexcept = default;
		ColorMatrix& operator=(const ColorMatrix&) noexcept = default;

		ColorMatrix() noexcept;
		ColorMatrix(const glm::vec4 & c0, const glm::vec4 & c1, const glm::vec4 & c2, const glm::vec4 & c3, const glm::vec4& c4) noexcept;
		
		static ColorMatrix from_rows(const std::array<float, 5> & r0, const std::array<float, 5>& r1, const std::array<float, 5>& r2, const std::array<float, 5>& r3) noexcept;

		static ColorMatrix hue_rotate(float angle) noexcept;

		static ColorMatrix saturate(float saturation) noexcept;

		static ColorMatrix luminance_to_alpha() noexcept;
	};
};

pf::ColorMatrix operator+(const pf::ColorMatrix& lh, const pf::ColorMatrix& rh);
pf::ColorMatrix operator-(const pf::ColorMatrix& lh, const pf::ColorMatrix& rh);
pf::ColorMatrix operator*(const pf::ColorMatrix& lh, float rh);