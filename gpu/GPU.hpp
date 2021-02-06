#pragma once
#include <cinttypes>
#include <iosfwd>
#include <string_view>

namespace pf {
	enum class TextureFormat {
		R8,
		R16F,
		RGBA8,
		RGBA16F,
		RGBA32F,
	};

	enum class VertexAttrType {
		F32,
		I8,
		I16,
		I32,
		U8,
		U16,
	};

	enum class BufferTarget {
		Vertex,
		Index,
		Storage,
	};

	enum class BufferUploadMode {
		Static,
		Dynamic,
	};

	enum class ShaderKind {
		Vertex,
		Fragment,
		Compute,
	};

	enum class Primitive {
		Triangles,
		Lines,
	};

	enum class BlendFactor {
		Zero,
		One,
		SrcAlpha,
		OneMinusSrcAlpha,
		DestAlpha,
		OneMinusDestAlpha,
		DestColor,
	};

	enum class BlendOp {
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max,
	};

	enum class DepthFunc {
		Less,
		Always,
	};

	enum class StencilFunc {
		Always,
		Equal,
	};

	enum class VertexAttrClass {
		Float,
		FloatNorm,
		Int,
	};

	enum class ImageAccess {
		Read,
		Write,
		ReadWrite,
	};

	enum class TextureSamplingFlags: uint8_t {
		None = 0,
		RepeatU = 1,
		RepeatV = 1 << 1,
		NearestMin = 1 << 2,
		NearestMag = 1 << 3,
		All = 0xF,
	};


	std::string_view to_string_view(pf::TextureFormat val);
	std::string_view to_string_view(pf::VertexAttrType val);
	std::string_view to_string_view(pf::BufferUploadMode val);
	std::string_view to_string_view(pf::ShaderKind val);
	std::string_view to_string_view(pf::Primitive val);
	std::string_view to_string_view(pf::BlendFactor val);
	std::string_view to_string_view(pf::BlendOp val);
	std::string_view to_string_view(pf::DepthFunc val);
	std::string_view to_string_view(pf::StencilFunc val);
	std::string_view to_string_view(pf::VertexAttrClass val);
	std::string_view to_string_view(pf::ImageAccess val);

	std::string to_string(pf::TextureFormat val);
	std::string to_string(pf::VertexAttrType val);
	std::string to_string(pf::BufferUploadMode val);
	std::string to_string(pf::ShaderKind val);
	std::string to_string(pf::Primitive val);
	std::string to_string(pf::BlendFactor val);
	std::string to_string(pf::BlendOp val);
	std::string to_string(pf::DepthFunc val);
	std::string to_string(pf::StencilFunc val);
	std::string to_string(pf::VertexAttrClass val);
	std::string to_string(pf::ImageAccess val);
};

pf::TextureSamplingFlags operator|(pf::TextureSamplingFlags lh, pf::TextureSamplingFlags rh) noexcept;
pf::TextureSamplingFlags operator&(pf::TextureSamplingFlags lh, pf::TextureSamplingFlags rh) noexcept;
pf::TextureSamplingFlags operator^(pf::TextureSamplingFlags lh, pf::TextureSamplingFlags rh) noexcept;
pf::TextureSamplingFlags operator~(pf::TextureSamplingFlags lh) noexcept;
pf::TextureSamplingFlags& operator|=(pf::TextureSamplingFlags& lh, pf::TextureSamplingFlags rh) noexcept;
pf::TextureSamplingFlags& operator&=(pf::TextureSamplingFlags& lh, pf::TextureSamplingFlags rh) noexcept;
pf::TextureSamplingFlags& operator^=(pf::TextureSamplingFlags& lh, pf::TextureSamplingFlags rh) noexcept;

std::ostream& operator<<(std::ostream& os, pf::TextureFormat val);
std::ostream& operator<<(std::ostream& os, pf::VertexAttrType val);
std::ostream& operator<<(std::ostream& os, pf::BufferUploadMode val);
std::ostream& operator<<(std::ostream& os, pf::ShaderKind val);
std::ostream& operator<<(std::ostream& os, pf::Primitive val);
std::ostream& operator<<(std::ostream& os, pf::BlendFactor val);
std::ostream& operator<<(std::ostream& os, pf::BlendOp val);
std::ostream& operator<<(std::ostream& os, pf::DepthFunc val);
std::ostream& operator<<(std::ostream& os, pf::StencilFunc val);
std::ostream& operator<<(std::ostream& os, pf::VertexAttrClass val);
std::ostream& operator<<(std::ostream& os, pf::ImageAccess val);
std::ostream& operator<<(std::ostream& os, pf::TextureSamplingFlags val);