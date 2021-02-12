#pragma once
#include <cinttypes>
#include <vector>
#include <memory>
#include <string_view>
#include <string>
#include <optional>

#include <glm/vec2.hpp>

#include "../gpu/Enums.hpp"
#include "../gpu/GPU.hpp"

#include "Types.hpp"

namespace pf {
	enum class GLVersion {
		gl3,
		gles3,
		gl4,
	};

	struct GLDevice {
		GLDevice(GLVersion ver, uint32_t fb);

		void setDefaultFramebuffer(uint32_t fb);
		void setRenderState();
		void setComputeState();
		void bindTexturesAndImages(GLProgram & program);
		void setRenderOptions(const RenderOptions & options);
		void setUniform(const GLUniform& uniform, const UniformData& data);
		void setStorageBuffer(GLStorageBuffer& storage, GLBuffer & buffer);
		void unsetStorageBuffer(GLStorageBuffer& storage);

		void resetRenderState();
		void resetComputeState();
		void resetRenderOptions(const RenderOptions& options);

		// impl Device abstraction
		std::string_view backendName() const;
		std::string deviceName() const;

		FeatureLevel featureLevel() const;

		GLTexture createTexture(TextureFormat format, glm::ivec2 size);
		GLShader createShader(std::string_view name, std::string_view source, ShaderKind kind);
		GLVertexArray createVertexArray();
		std::optional<GLVertexAttr> getVertexAttr(const GLProgram & program, std::string_view name);
		GLUniform getUniform(const GLProgram& program, std::string_view name);
		GLTextureParam getTextureParam(const GLProgram& program, std::string_view name);
		GLImageParam getImageParam(const GLProgram& program, std::string_view name);
		GLStorageBuffer getStorageBuffer(const GLProgram& program, std::string_view name);
		GLFramebuffer createFramebuffer(GLTexture && texture);
		GLBuffer createBuffer(BufferUploadMode mode);


		GLVersion version;
		uint32_t defaultFramebuffer;
		GLTexture dummyTexture;
	};

	
};