#include "Util.hpp"
#include <GL/glew.h>

#include <cstdio>
#include <fmt/core.h>


namespace pf {
	int32_t convertGL(BlendFactor val) noexcept {
		switch (val) {
		case BlendFactor::DestAlpha:
			return GL_DST_ALPHA;
		case BlendFactor::DestColor:
			return GL_DST_COLOR;
		case BlendFactor::One:
			return GL_ONE;
		case BlendFactor::OneMinusDestAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case BlendFactor::OneMinusSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case BlendFactor::SrcAlpha:
			return GL_SRC_ALPHA;
		case BlendFactor::Zero:
			return GL_ZERO;
		}
	}
	int32_t convertGL(BlendOp val) noexcept {
		switch (val) {
		case BlendOp::Add:
			return GL_FUNC_ADD;
		case BlendOp::Subtract:
			return GL_FUNC_SUBTRACT;
		case BlendOp::ReverseSubtract:
			return GL_FUNC_REVERSE_SUBTRACT;
		case BlendOp::Min:
			return GL_MIN;
		case BlendOp::Max:
			return GL_MAX;
		}
	}
	int32_t convertGL(BufferTarget val) noexcept {
		switch (val) {
		case BufferTarget::Index:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BufferTarget::Storage:
			return GL_SHADER_STORAGE_BUFFER;
		case BufferTarget::Vertex:
			return GL_ARRAY_BUFFER;
		}
	}
	int32_t convertGL(BufferUploadMode val) noexcept {
		switch (val) {
		case BufferUploadMode::Dynamic:
			return GL_DYNAMIC_DRAW;
		case BufferUploadMode::Static:
			return GL_STATIC_DRAW;
		}
	}
	int32_t convertGL(DepthFunc val) noexcept {
		switch (val) {
		case DepthFunc::Always:
			return GL_ALWAYS;
		case DepthFunc::Less:
			return GL_LESS;
		}
	}
	int32_t convertGL(ImageAccess val) noexcept {
		switch (val) {
		case ImageAccess::Read:
			return GL_READ_ONLY;
		case ImageAccess::ReadWrite:
			return GL_READ_WRITE;
		case ImageAccess::Write:
			return GL_WRITE_ONLY;
		}
	}
	int32_t convertGL(Primitive val) noexcept {
		switch (val) {
		case Primitive::Lines:
			return GL_LINES;
		case Primitive::Triangles:
			return GL_TRIANGLES;
		}
	}
	int32_t convertGL(StencilFunc val) noexcept {
		switch (val) {
		case StencilFunc::Always:
			return GL_ALWAYS;
		case StencilFunc::Equal:
			return GL_EQUAL;
		}
	}
	int32_t convertGL(VertexAttrType val) noexcept {
		switch (val) {
		case VertexAttrType::F32:
			return GL_FLOAT;
		case VertexAttrType::I8:
			return GL_BYTE;
		case VertexAttrType::I16:
			return GL_SHORT;
		case VertexAttrType::I32:
			return GL_INT;
		case VertexAttrType::U8:
			return GL_UNSIGNED_BYTE;
		case VertexAttrType::U16:
			return GL_UNSIGNED_SHORT;
		}
	}
	int32_t convertGL(ShaderKind val) noexcept {
		switch (val) {
		case ShaderKind::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderKind::Fragment:
			return GL_FRAGMENT_SHADER;
		case ShaderKind::Compute:
			return GL_COMPUTE_SHADER;
		}
	}

	int32_t glInternalFormat(TextureFormat val) noexcept {
		switch (val) {
		case TextureFormat::R8:
			return GL_R8;
		case TextureFormat::R16F:
			return GL_R16F;
		case TextureFormat::RGBA8:
			return GL_RGBA8;
		case TextureFormat::RGBA16F:
			return GL_RGBA16F;
		case TextureFormat::RGBA32F:
			return GL_RGBA32F;
		}
	}
	uint32_t glFormat(TextureFormat val) noexcept {
		switch (val) {
		case TextureFormat::R8:
		case TextureFormat::R16F:
			return GL_RED;
		case TextureFormat::RGBA8:
		case TextureFormat::RGBA16F:
		case TextureFormat::RGBA32F:
			return GL_RGBA;
		}
	}
	uint32_t glType(TextureFormat val) noexcept {
		switch (val) {
		case TextureFormat::R8:
		case TextureFormat::RGBA8:
			return GL_UNSIGNED_BYTE;
		case TextureFormat::R16F:
			return GL_HALF_FLOAT;
		case TextureFormat::RGBA32F:
			return GL_FLOAT;
		}
	}

	void glCheckErrors() {
#ifdef PF_DEBUG_ASSERTIONS
		int errCount = 0;
		GLenum err = glGetError();
		while (err != GL_NO_ERROR) {
			fmt::print(stderr, "GL error: 0x{:x}");
			const char* text = "Unknown";
			switch (err) {
			case GL_INVALID_ENUM:
				text = "GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				text = "GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				text = "GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				text = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				text = "GL_OUT_OF_MEMORY";
				break;
			case GL_STACK_UNDERFLOW:
				text = "GL_STACK_UNDERFLOW";
				break;
			case GL_STACK_OVERFLOW:
				text = "GL_STACK_OVERFLOW";
				break;
			}

			fmt::print(stderr, " ({})\n", text);
			err = glGetError();
			++errCount;
		}
		
		assert(errCount == 0);
#endif
	}
};