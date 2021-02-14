#pragma once
#include "../gpu/GPU.hpp"

namespace pf {
	int32_t convertGL(BlendFactor val) noexcept;
	int32_t convertGL(BlendOp val) noexcept;
	int32_t convertGL(BufferTarget val) noexcept;
	int32_t convertGL(BufferUploadMode val) noexcept;
	int32_t convertGL(DepthFunc val) noexcept;
	int32_t convertGL(ImageAccess val) noexcept;
	int32_t convertGL(Primitive val) noexcept;
	int32_t convertGL(StencilFunc val) noexcept;
	int32_t convertGL(VertexAttrType val) noexcept;
	int32_t convertGL(ShaderKind val) noexcept;

	int32_t glInternalFormat(TextureFormat val) noexcept;
	uint32_t glFormat(TextureFormat val) noexcept;
	uint32_t glType(TextureFormat val) noexcept;

	void glCheckErrors();
};