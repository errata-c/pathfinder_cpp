#pragma once
#include <cinttypes>
#include <vector>
#include <memory>

#include <glm/vec2.hpp>

#include "../gpu/Enums.hpp"

namespace pf {
	struct GLTimerQuery {
		GLTimerQuery(uint32_t _id);
		~GLTimerQuery();

		GLTimerQuery(GLTimerQuery&& other) noexcept;
		GLTimerQuery& operator=(GLTimerQuery&& other) noexcept;

		const uint32_t id() const noexcept;
	private:
		uint32_t ref;
	};
	struct GLTexture {
		GLTexture(uint32_t _id);
		~GLTexture();

		GLTexture(GLTexture&& other) noexcept;
		GLTexture& operator=(GLTexture&& other) noexcept;

		const uint32_t id() const noexcept;

		uint32_t ref;
		glm::ivec2 size;
	};
	struct GLShader {
		GLShader();
		~GLShader();
		GLShader(GLShader&& other) noexcept;
		GLShader& operator=(GLShader&& other) noexcept;

		const uint32_t id() const noexcept;

	private:
		uint32_t ref;
	};
	struct GLVertexArray {
		GLVertexArray(uint32_t _id);
		~GLVertexArray();

		GLVertexArray(GLVertexArray&& other) noexcept;
		GLVertexArray& operator=(GLVertexArray&& other) noexcept;

		const uint32_t id() const noexcept;

	private:
		uint32_t ref;
	};

	struct GLVertexAttr {
		~GLVertexAttr() = default;
		GLVertexAttr(GLVertexAttr&& other) noexcept = default;
		GLVertexAttr& operator=(GLVertexAttr&& other) noexcept = default;

		GLVertexAttr(uint32_t _ref);

		const uint32_t index() const noexcept;
	private:
		uint32_t ref;
	};

	struct GLFence {
		GLFence(struct __GLsync* _ref) noexcept;

		~GLFence();

		GLFence(GLFence&& other) noexcept;
		GLFence& operator=(GLFence&& other) noexcept;

		struct __GLsync* id() noexcept;
		struct __GLsync const* id() const noexcept;
	private:
		struct __GLsync* ref;
	};

	struct GLFramebuffer {
		GLFramebuffer(uint32_t _ref);
		~GLFramebuffer();

		GLFramebuffer(GLFramebuffer&& other) noexcept;
		GLFramebuffer& operator=(GLFramebuffer&& other) noexcept;

		const uint32_t id() const noexcept;

		GLTexture texture;
	private:
		uint32_t ref;
	};

	struct GLBufferObject {
		GLBufferObject(uint32_t _ref);
		~GLBufferObject();

		GLBufferObject(GLBufferObject&& other) noexcept;
		GLBufferObject& operator=(GLBufferObject&& other) noexcept;

		const uint32_t id() const noexcept;
	private:
		uint32_t ref;
	};

	struct GLBuffer {
		std::shared_ptr<GLBufferObject> object;
		BufferUploadMode mode;
	};

	struct GLStorageBuffer {
		int32_t location;
	};

	struct GLUniform {
		GLUniform(uint32_t _ref);

		~GLUniform();
		GLUniform(GLUniform&& other) noexcept;
		GLUniform& operator=(GLUniform&& other) noexcept;

		const uint32_t id() const noexcept;
	private:
		uint32_t ref;
	};

	struct GLTextureParam {
		GLUniform uniform;
		uint32_t unit;
	};

	struct GLImageParam {
		GLUniform uniform;
		uint32_t unit;
	};

	struct GLProgramParams {
		std::vector<GLUniform> textures;
		std::vector<GLUniform> images;
	};

	struct GLProgram {
		GLProgram(uint32_t _ref);
		~GLProgram();

		GLProgram(GLProgram&& other) noexcept;
		GLProgram& operator=(GLProgram&& other) noexcept;

		const uint32_t id() const noexcept;

		GLProgramParams params;
	private:
		uint32_t ref;
	};
}