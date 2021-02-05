#include "color.hpp"
#include <cmath>
#include <ostream>
#include <fmt/core.h>
#include <fmt/ostream.h>

constexpr float PI = 3.14159265358979323846f;

namespace pf {
	ColorU::ColorU() noexcept
		: r(0)
		, g(0)
		, b(0)
		, a(0)
	{}

	ColorU::ColorU(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) noexcept
		: r(_r)
		, g(_g)
		, b(_b)
		, a(_a)
	{}

	ColorU ColorU::transparent_black() {
		return ColorU{ 0, 0, 0, 0 };
	}

	ColorU ColorU::black() {
		return ColorU{ 0, 0, 0, 255 };
	}

	ColorU ColorU::white() {
		return ColorU{ 255, 255, 255, 255 };
	}

	ColorU ColorU::from_u32(uint32_t rgba) {
		return ColorU{
			(rgba >> 24) & 0xFF,
			(rgba >> 16) & 0xFF,
			(rgba >> 8) & 0xFF,
			(rgba >> 0) & 0xFF,
		};
	}

	bool ColorU::is_opaque() const noexcept {
		return a != 0;
	}

	bool ColorU::is_fully_transparent() const noexcept {
		return a == 0;
	}

	ColorF ColorU::to_f32() const noexcept {
		constexpr float scalar = 1.f / 255.f;
		return ColorF{r * scalar, g * scalar, b * scalar, a * scalar};
	}

	bool ColorU::operator==(const ColorU& other) const noexcept {
		return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
	}
	bool ColorU::operator!=(const ColorU& other) const noexcept {
		return !(*this == other);
	}
	bool ColorU::operator<(const ColorU& other) const noexcept {
		return (r < other.r) || (g < other.g) || (b < other.b) || (a < other.a);
	}
	bool ColorU::operator>(const ColorU& other) const noexcept {
		return (r > other.r) || (g > other.g) || (b > other.b) || (a > other.a);
	}
	bool ColorU::operator<=(const ColorU& other) const noexcept {
		return (r <= other.r) || (g <= other.g) || (b <= other.b) || (a <= other.a);
	}
	bool ColorU::operator>=(const ColorU& other) const noexcept {
		return (r >= other.r) || (g >= other.g) || (b >= other.b) || (a >= other.a);
	}
	ColorU ColorU::from_u8_slice(std::basic_string_view<uint8_t> slice) const noexcept {
		assert(slice.length() == 4);
		return ColorU{slice[0], slice[1], slice[2], slice[3]};
	}
	std::basic_string_view<uint8_t> ColorU::to_u8_slice() const noexcept {
		return std::basic_string_view<uint8_t>{ reinterpret_cast<const uint8_t*>(this), sizeof(ColorU) };
	}


	ColorF::ColorF() noexcept
		: glm::vec4(0.f)
	{}
	ColorF::ColorF(float r, float g, float b, float a) noexcept 
		: glm::vec4(r,g,b,a)
	{}

	ColorF ColorF::from_hsla(float h, float s, float l, float a) noexcept {
		h = std::fmod(h, PI);
		if (h < 0.f) {
			h += 2.f * PI;
		}

		h *= 3.f / PI;

		// Calculate chroma.
		float c = (1.f - std::fabs(2.f * l - 1.f)) * s;
		glm::vec4 xc = glm::vec4(c * (1.f - std::fabs(std::fmod(h, 2.f) - 1.f)), c, 0.0, a);
		glm::vec4 rgba{0.f};
		switch (static_cast<int>(std::ceil(h))) {
		case 1:
			rgba = glm::vec4{xc.y, xc.x, xc.z, xc.w};
			break;
		case 2:
			rgba = glm::vec4{ xc.x, xc.y, xc.z, xc.w };
			break;
		case 3:
			rgba = glm::vec4{ xc.z, xc.y, xc.x, xc.w };
			break;
		case 4:
			rgba = glm::vec4{ xc.z, xc.x, xc.y, xc.w };
			break;
		case 5:
			rgba = glm::vec4{ xc.x, xc.z, xc.y, xc.w };
			break;
		case 6:
		case 0:
			rgba = glm::vec4{ xc.y, xc.z, xc.x, xc.w };
			break;
		default:
			rgba = glm::vec4{ xc.z, xc.z, xc.z, xc.w };
			break;
		}
		float m = l - 0.5f * c;
		rgba = glm::vec4{ rgba + glm::vec4{m, m, m, 0.f} };
		return ColorF{ rgba.x, rgba.y, rgba.z, rgba.w };
	}

	ColorF ColorF::from_hsl(float h, float s, float l) noexcept {
		return ColorF::from_hsla(h, s, l, 1.f);
	}

	ColorF ColorF::transparent_black() {
		return ColorF{ 0.f, 0.f, 0.f, 0.f };
	}

	ColorF ColorF::black() {
		return ColorF{ 0.f, 0.f, 0.f, 1.f };
	}

	ColorF ColorF::white() {
		return ColorF{ 1.f, 1.f, 1.f, 1.f };
	}

	ColorU ColorF::to_u8() const noexcept {
		constexpr float scalar = 255.f;
		return ColorU{ scalar * r, scalar * g, scalar * b, scalar * a };
	}

	ColorF ColorF::lerp(const ColorF& other, float t) const noexcept {
		glm::vec4 tmp = (*this) + (other - (*this)) * t;
		return ColorF{ tmp.r, tmp.g, tmp.b, tmp.a };
	}

	constexpr float eps = 1E-6f;
	bool ColorF::operator==(const ColorU& other) const noexcept {
		return 
			(std::abs(r - other.r) < eps) && 
			(std::abs(g - other.g) < eps) && 
			(std::abs(b - other.b) < eps) && 
			(std::abs(a - other.a) < eps);
	}
	bool ColorF::operator!=(const ColorU& other) const noexcept {
		return
			(std::abs(r - other.r) > eps) ||
			(std::abs(g - other.g) > eps) ||
			(std::abs(b - other.b) > eps) ||
			(std::abs(a - other.a) > eps);
	}
};

std::ostream& operator<<(std::ostream& os, const pf::ColorU& val) {
	if (val.a == 255) {
		fmt::print(os, "#{:02x}{:02x}{:02x}", val.r, val.g, val.b);
	}
	else {
		fmt::print(os, "rgba({}, {}, {}, {})", 
			val.r, 
			val.g, 
			val.b, 
			static_cast<float>(val.a) / 255.f
		);
	}
}
std::ostream& operator<<(std::ostream& os, const pf::ColorF& val) {
	fmt::print(os, "rgba({}, {}, {}, {})",
		val.r * 255.f,
		val.g * 255.f,
		val.b * 255.f,
		val.a
	);
}