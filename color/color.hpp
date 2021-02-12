#pragma once
#include <cinttypes>
#include <iosfwd>
#include <string_view>

#include <glm/vec4.hpp>

namespace pf {
	struct ColorF;

	struct ColorU {
		uint8_t r, g, b, a;

		ColorU(const ColorU&) noexcept = default;
		ColorU& operator=(const ColorU&) noexcept = default;
		~ColorU() = default;

		ColorU() noexcept;
		ColorU(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255) noexcept;
		
		static ColorU transparent_black();

		static ColorU black();

		static ColorU white();

		static ColorU from_u32(uint32_t rgba);

		bool is_opaque() const noexcept;

		bool is_fully_transparent() const noexcept;
	
		ColorF to_f32() const noexcept;

		bool operator==(const ColorU& other) const noexcept;
		bool operator!=(const ColorU& other) const noexcept;
		bool operator<(const ColorU& other) const noexcept;
		bool operator>(const ColorU& other) const noexcept;
		bool operator<=(const ColorU& other) const noexcept;
		bool operator>=(const ColorU& other) const noexcept;

		ColorU from_u8_slice(std::basic_string_view<uint8_t> slice) const noexcept;
		std::basic_string_view<uint8_t> to_u8_slice() const noexcept;
	};

	struct ColorF: public glm::vec4 {
		ColorF(const ColorF&) noexcept = default;
		ColorF& operator=(const ColorF&) noexcept = default;
		~ColorF() = default;

		ColorF() noexcept;
		ColorF(float r, float g, float b, float a = 1.f) noexcept;

		static ColorF from_hsla(float h, float s, float l, float a) noexcept;

		static ColorF from_hsl(float h, float s, float l) noexcept;

		static ColorF transparent_black();

		static ColorF black();

		static ColorF white();

		ColorU to_u8() const noexcept;

		ColorF lerp(const ColorF& other, float t) const noexcept;

		bool operator==(const ColorU& other) const noexcept;
		bool operator!=(const ColorU& other) const noexcept;
	};
};

std::ostream& operator<<(std::ostream& os, const pf::ColorU& val);
std::ostream& operator<<(std::ostream& os, const pf::ColorF& val);