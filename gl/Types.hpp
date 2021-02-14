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
		GLTexture(uint32_t _id = 0);
		GLTexture(uint32_t _id, glm::ivec2 _size);
		~GLTexture();

		GLTexture(GLTexture&& other) noexcept;
		GLTexture& operator=(GLTexture&& other) noexcept;

		const uint32_t id() const noexcept;

		uint32_t ref;
		glm::ivec2 size;
		TextureFormat format;
	};
	struct GLShader {
		GLShader(uint32_t _ref = 0);
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

		GLVertexAttr(int32_t _ref);

		const int32_t id() const noexcept;
	private:
		int32_t ref;
	};

	struct GLFence {
		GLFence(void* _ref) noexcept;

		~GLFence();

		GLFence(GLFence&& other) noexcept;
		GLFence& operator=(GLFence&& other) noexcept;

		void* id() noexcept;
		void const* id() const noexcept;
	private:
		void* ref;
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
		GLUniform(const GLUniform&) noexcept = default;
		GLUniform& operator=(const GLUniform&) noexcept = default;
		~GLUniform() = default;

		GLUniform(uint32_t _ref);

		bool operator==(const GLUniform& other) const noexcept;
		bool operator!=(const GLUniform& other) const noexcept;

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

		std::unique_ptr<GLProgramParams> params;
	private:
		uint32_t ref;
	};

	struct GLBufferDataReceiver {
		GLBufferDataReceiver(const std::shared_ptr<GLBufferObject> & obj, BufferTarget tar, std::size_t b, std::size_t e);
		GLBufferDataReceiver(GLBufferDataReceiver && other) noexcept;
		~GLBufferDataReceiver();

		GLBufferDataReceiver& operator=(GLBufferDataReceiver&& other) noexcept;

		std::shared_ptr<GLBufferObject> object;
		std::size_t begin, end;
		BufferTarget target;
		GLFence sync;
	};

	struct GLTextureDataReceiver {
		GLTextureDataReceiver(uint32_t pix, glm::ivec2 _size, TextureFormat form);
		GLTextureDataReceiver(GLTextureDataReceiver&& other) noexcept;
		~GLTextureDataReceiver();

		GLTextureDataReceiver& operator=(GLTextureDataReceiver&& other) noexcept;

		uint32_t pixelBuffer;
		glm::ivec2 size;
		TextureFormat format;
		GLFence sync;
	};
}