#pragma once
#include <cinttypes>
#include <vector>
#include <glm/vec2.hpp>

namespace pf {
	enum class GLVersion {
		gl3,
		gles3,
		gl4,
	};

	struct GLDevice {

	};

	struct GLTimerQuery {
		uint32_t id;
	};
	struct GLTexture {
		uint32_t id;
		glm::ivec2 size;
	};
	struct GLShader {
		uint32_t id;
	};
	struct GLVertexArray {
		uint32_t id;
	};
	struct GLVertexAttrib {
		uint32_t id;
	};
	struct GLFence {
		uint32_t id;
	};
	struct GLFramebuffer {
		uint32_t id;
		GLTexture texture;
	};
	struct GLBufferObject {
		uint32_t id;
	};
	struct GLUniform {
		uint32_t id;
	};

	struct GLTextureParam {
		GLUniform uniform;
		uint32_t unit;
	};
	struct GLImageParam {
		GLUniform uniform;
		uint32_t unit;
	};

	struct GLProgram {
		uint32_t id;
	};

	struct GLProgramParams {
		std::vector<GLUniform> textures;
		std::vector<GLUniform> images;
	};
};