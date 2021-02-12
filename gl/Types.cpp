#include "Types.hpp"
#include <gl/glew.h>

namespace pf {
	/*
	GLTimerQuery(uint32_t _id);
	~GLTimerQuery();

	GLTimerQuery(GLTimerQuery&& other) noexcept;
	GLTimerQuery& operator=(GLTimerQuery&& other) noexcept;

	const uint32_t id() const noexcept;
	*/

	/*
	GLTexture(uint32_t _id);
	~GLTexture();

	GLTexture(GLTexture&& other) noexcept;
	GLTexture& operator=(GLTexture&& other) noexcept;

	const uint32_t id() const noexcept;
	*/

	/*
	GLShader();
	~GLShader();
	GLShader(GLShader&& other) noexcept;
	GLShader& operator=(GLShader&& other) noexcept;
	*/


	/*
	GLVertexArray(uint32_t _id);
	~GLVertexArray();

	GLVertexArray(GLVertexArray&& other) noexcept;
	GLVertexArray& operator=(GLVertexArray&& other) noexcept;
	*/
	
	
	/*
	GLFence(struct __GLsync* _ref) noexcept;

	~GLFence();

	GLFence(GLFence&& other) noexcept;
	GLFence& operator=(GLFence&& other) noexcept;
	*/


	/*
	GLFramebuffer(uint32_t _ref);
	~GLFramebuffer();

	GLFramebuffer(GLFramebuffer&& other) noexcept;
	GLFramebuffer& operator=(GLFramebuffer&& other) noexcept;
	*/

	/*
	GLBufferObject(uint32_t _ref);
	~GLBufferObject();

	GLBufferObject(GLBufferObject&& other) noexcept;
	GLBufferObject& operator=(GLBufferObject&& other) noexcept;

	const uint32_t id() const noexcept;
	*/

	GLUniform::GLUniform(uint32_t _ref)
		: ref(_ref)
	{}
	GLUniform::~GLUniform() {}

	GLUniform::GLUniform(GLUniform&& other) noexcept
		: ref(other.ref)
	{
		other.ref = 0;
	}
	GLUniform& GLUniform::operator=(GLUniform&& other) noexcept {
		GLUniform::~GLUniform();
		ref = other.ref;
		other.ref = 0;
		return *this;
	}
	const uint32_t GLUniform::id() const noexcept {
		return ref;
	}


	GLProgram::GLProgram(uint32_t _ref)
		: ref(_ref)
	{}
	GLProgram::~GLProgram() {
		if (ref != 0) {
			glDeleteProgram(ref);
			ref = 0;
		}
	}

	GLProgram::GLProgram(GLProgram&& other) noexcept 
		: ref(other.ref)
	{
		other.ref = 0;
	}
	GLProgram& GLProgram::operator=(GLProgram&& other) noexcept {
		GLProgram::~GLProgram();
		ref = other.ref;
		other.ref = 0;
		return *this;
	}

	const uint32_t GLProgram::id() const noexcept {
		return ref;
	}
};