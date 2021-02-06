#include "GPU.hpp"
#include <ostream>

template<std::size_t N>
constexpr std::string_view makeView(const char(&text)[N]) {
	return std::string_view{ text, N-1 };
}

namespace pf {
	std::string_view to_string_view(pf::TextureFormat val) {
		switch (val) {
		case TextureFormat::R8:
			return makeView("R8");
		case TextureFormat::R16F:
			return makeView("R16F");
		case TextureFormat::RGBA8:
			return makeView("RGBA8");
		case TextureFormat::RGBA16F:
			return makeView("RGBA16F");
		case TextureFormat::RGBA32F:
			return makeView("RGBA32F");
		}
	}
	std::string_view to_string_view(pf::VertexAttrType val) {
		switch (val) {
		case VertexAttrType::F32:
			return makeView("F32");
		case VertexAttrType::I16:
			return makeView("I16");
		case VertexAttrType::I32:
			return makeView("I32");
		case VertexAttrType::I8:
			return makeView("I8");
		case VertexAttrType::U16:
			return makeView("U16");
		case VertexAttrType::U8:
			return makeView("U8");
		}
	}
	std::string_view to_string_view(pf::BufferUploadMode val) {
		switch (val) {
		case BufferUploadMode::Dynamic:
			return makeView("Dynamic");
		case BufferUploadMode::Static:
			return makeView("Static");
		}
	}
	std::string_view to_string_view(pf::ShaderKind val) {
		switch (val) {
		case ShaderKind::Compute:
			return makeView("Compute");
		case ShaderKind::Fragment:
			return makeView("Fragment");
		case ShaderKind::Vertex:
			return makeView("Vertex");
		}
	}
	std::string_view to_string_view(pf::Primitive val) {
		switch (val) {
		case Primitive::Lines:
			return makeView("Lines");
		case Primitive::Triangles:
			return makeView("Triangles");
		}
	}
	std::string_view to_string_view(pf::BlendFactor val) {
		switch (val) {
		case BlendFactor::DestAlpha:
			return makeView("DestAlpha");
		case BlendFactor::DestColor:
			return makeView("DestColor");
		case BlendFactor::One:
			return makeView("One");
		case BlendFactor::OneMinusDestAlpha:
			return makeView("OneMinusDestAlpha");
		case BlendFactor::OneMinusSrcAlpha:
			return makeView("OneMinusSrcAlpha");
		case BlendFactor::SrcAlpha:
			return makeView("SrcAlpha");
		case BlendFactor::Zero:
			return makeView("Zero");
		}
	}
	std::string_view to_string_view(pf::BlendOp val) {
		switch (val) {
		case BlendOp::Add:
			return makeView("Add");
		case BlendOp::Max:
			return makeView("Max");
		case BlendOp::Min:
			return makeView("Min");
		case BlendOp::ReverseSubtract:
			return makeView("ReverseSubtract");
		case BlendOp::Subtract:
			return makeView("Subtract");
		}
	}
	std::string_view to_string_view(pf::DepthFunc val) {
		switch (val) {
		case DepthFunc::Always:
			return makeView("Always");
		case DepthFunc::Less:
			return makeView("Less");
		}
	}
	std::string_view to_string_view(pf::StencilFunc val) {
		switch (val) {
		case StencilFunc::Always:
			return makeView("Always");
		case StencilFunc::Equal:
			return makeView("Equal");
		}
	}
	std::string_view to_string_view(pf::VertexAttrClass val) {
		switch (val) {
		case VertexAttrClass::Float:
			return makeView("Float");
		case VertexAttrClass::FloatNorm:
			return makeView("FloatNorm");
		case VertexAttrClass::Int:
			return makeView("Int");
		}
	}
	std::string_view to_string_view(pf::ImageAccess val) {
		switch (val) {
		case ImageAccess::Read:
			return makeView("Read");
		case ImageAccess::ReadWrite:
			return makeView("ReadWrite");
		case ImageAccess::Write:
			return makeView("Write");
		}
	}

	std::string to_string(pf::TextureFormat val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::VertexAttrType val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::BufferUploadMode val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::ShaderKind val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::Primitive val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::BlendFactor val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::BlendOp val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::DepthFunc val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::StencilFunc val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::VertexAttrClass val) {
		return std::string{ pf::to_string_view(val) };
	}
	std::string to_string(pf::ImageAccess val) {
		return std::string{ pf::to_string_view(val) };
	}
};

pf::TextureSamplingFlags operator|(pf::TextureSamplingFlags lh, pf::TextureSamplingFlags rh) noexcept {
	return static_cast<pf::TextureSamplingFlags>(
			static_cast<int>(lh) | static_cast<int>(rh)
		);
}
pf::TextureSamplingFlags operator&(pf::TextureSamplingFlags lh, pf::TextureSamplingFlags rh) noexcept {
	return static_cast<pf::TextureSamplingFlags>(
		static_cast<int>(lh) & static_cast<int>(rh)
		);
}
pf::TextureSamplingFlags operator^(pf::TextureSamplingFlags lh, pf::TextureSamplingFlags rh) noexcept {
	return static_cast<pf::TextureSamplingFlags>(
		static_cast<int>(lh) ^ static_cast<int>(rh)
		) & pf::TextureSamplingFlags::All;
}
pf::TextureSamplingFlags operator~(pf::TextureSamplingFlags lh) noexcept {
	return static_cast<pf::TextureSamplingFlags>(
		~static_cast<int>(lh)
		) & pf::TextureSamplingFlags::All;
}
pf::TextureSamplingFlags& operator|=(pf::TextureSamplingFlags& lh, pf::TextureSamplingFlags rh) noexcept {
	lh = lh | rh;
	return lh;
}
pf::TextureSamplingFlags& operator&=(pf::TextureSamplingFlags& lh, pf::TextureSamplingFlags rh) noexcept {
	lh = lh & rh;
	return lh;
}
pf::TextureSamplingFlags& operator^=(pf::TextureSamplingFlags& lh, pf::TextureSamplingFlags rh) noexcept {
	lh = lh ^ rh;
	return lh;
}

std::ostream& operator<<(std::ostream& os, pf::TextureFormat val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::VertexAttrType val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::BufferUploadMode val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::ShaderKind val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::Primitive val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::BlendFactor val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::BlendOp val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::DepthFunc val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::StencilFunc val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::VertexAttrClass val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::ImageAccess val) {
	os << pf::to_string_view(val);
	return os;
}
std::ostream& operator<<(std::ostream& os, pf::TextureSamplingFlags val) {
	os << "[ ";

	if ((val & pf::TextureSamplingFlags::RepeatU) != pf::TextureSamplingFlags::None) {
		os << "RepeatU, ";
	}
	if ((val & pf::TextureSamplingFlags::RepeatV) != pf::TextureSamplingFlags::None) {
		os << "RepeatV, ";
	}
	if ((val & pf::TextureSamplingFlags::NearestMag) != pf::TextureSamplingFlags::None) {
		os << "NearestMag, ";
	}
	if ((val & pf::TextureSamplingFlags::NearestMin) != pf::TextureSamplingFlags::None) {
		os << "NearestMin, ";
	}
	os << "]";
	return os;
}