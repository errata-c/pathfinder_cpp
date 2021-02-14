#include "Types.hpp"
#include <gl/glew.h>

namespace pf {
	
	GLTimerQuery::GLTimerQuery(uint32_t _id) 
		: ref(_id)
	{}
	GLTimerQuery::~GLTimerQuery() {
		if (ref != 0) {
			ref = 0;
			glDeleteQueries(1, &ref);
		}
	}

	GLTimerQuery::GLTimerQuery(GLTimerQuery&& other) noexcept
		: ref(other.ref)
	{
		other.ref = 0;
	}
	GLTimerQuery& GLTimerQuery::operator=(GLTimerQuery&& other) noexcept {
		GLTimerQuery::~GLTimerQuery();
		ref = other.ref;
		other.ref = 0;
	}

	const uint32_t GLTimerQuery::id() const noexcept {
		return ref;
	}
	

	
	GLTexture::GLTexture(uint32_t _id) 
		: ref(_id)
		, size(0)
	{}
	GLTexture::GLTexture(uint32_t _id, glm::ivec2 _size)
		: ref(_id)
		, size(_size)
	{
		assert(ref != 0);
	}
	GLTexture::~GLTexture() {
		if (ref != 0) {
			glDeleteTextures(1, &ref);
			ref = 0;
		}
	}

	GLTexture::GLTexture(GLTexture&& other) noexcept 
		: ref(other.ref)
		, size(other.size)
	{
		other.ref = 0;
	}
	GLTexture& GLTexture::operator=(GLTexture&& other) noexcept {
		GLTexture::~GLTexture();
		ref = other.ref;
		size = other.size;
		other.ref = 0;
		return *this;
	}

	const uint32_t GLTexture::id() const noexcept {
		return ref;
	}
	

	
	GLShader::GLShader(uint32_t _ref) 
		: ref(_ref)
	{}
	GLShader::~GLShader() {
		if (ref != 0) {
			glDeleteShader(ref);
			ref = 0;
		}
	}
	GLShader::GLShader(GLShader&& other) noexcept
		: ref(other.ref)
	{
		other.ref = 0;
	}
	GLShader& GLShader::operator=(GLShader&& other) noexcept {
		GLShader::~GLShader();
		ref = other.ref;
		other.ref = 0;
		return *this;
	}

	const uint32_t GLShader::id() const noexcept {
		return ref;
	}

	
	GLVertexArray::GLVertexArray(uint32_t _id) 
		: ref(_id)
	{}
	GLVertexArray::~GLVertexArray() {
		if (ref != 0) {
			glDeleteVertexArrays(1, &ref);
			ref = 0;
		}
	}

	GLVertexArray::GLVertexArray(GLVertexArray&& other) noexcept 
		: ref(other.ref)
	{
		other.ref = 0;
	}
	GLVertexArray& GLVertexArray::operator=(GLVertexArray&& other) noexcept {
		GLVertexArray::~GLVertexArray();
		ref = other.ref;
		other.ref = 0;
		return *this;
	}
	
	
	
	
	GLFence::GLFence(void* _ref) noexcept 
		: ref(_ref)
	{}
	GLFence::~GLFence() {
		if (ref != nullptr) {
			glDeleteSync((GLsync)ref);
			ref = nullptr;
		}
	}
	GLFence::GLFence(GLFence&& other) noexcept
		: ref(other.ref)
	{
		other.ref = nullptr;
	}
	GLFence& GLFence::operator=(GLFence&& other) noexcept {
		GLFence::~GLFence();
		ref = other.ref;
		other.ref = nullptr;
		return *this;
	}
	void* GLFence::id() noexcept {
		return ref;
	}
	void const* GLFence::id() const noexcept {
		return ref;
	}
	

	GLFramebuffer::GLFramebuffer(uint32_t _ref) 
		: ref(_ref)
	{}
	GLFramebuffer::~GLFramebuffer() {
		if (ref != 0) {
			glDeleteFramebuffers(1, &ref);
		}
	}
	GLFramebuffer::GLFramebuffer(GLFramebuffer&& other) noexcept 
		: ref(other.ref)
		, texture(std::move(other.texture))
	{
		other.ref = 0;
	}
	GLFramebuffer& GLFramebuffer::operator=(GLFramebuffer&& other) noexcept {

		return *this;
	}
	const uint32_t GLFramebuffer::id() const noexcept {
		return ref;
	}

	
	GLBufferObject::GLBufferObject(uint32_t _ref) 
		: ref(_ref)
	{}
	GLBufferObject::~GLBufferObject() {
		if (ref != 0) {
			glDeleteBuffers(1, &ref);
			ref = 0;
		}
	}
	GLBufferObject::GLBufferObject(GLBufferObject&& other) noexcept 
		: ref(other.ref)
	{
		other.ref = 0;
	}
	GLBufferObject& GLBufferObject::operator=(GLBufferObject&& other) noexcept {
		GLBufferObject::~GLBufferObject();
		ref = other.ref;
		other.ref = 0;
		return *this;
	}
	const uint32_t GLBufferObject::id() const noexcept {
		return ref;
	}
	

	GLUniform::GLUniform(uint32_t _ref)
		: ref(_ref)
	{}
	const uint32_t GLUniform::id() const noexcept {
		return ref;
	}
	bool GLUniform::operator==(const GLUniform& other) const noexcept {
		return ref == other.ref;
	}
	bool GLUniform::operator!=(const GLUniform& other) const noexcept {
		return ref != other.ref;
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


	GLVertexAttr::GLVertexAttr(int32_t _ref)
		: ref(_ref)
	{}
	const int32_t GLVertexAttr::id() const noexcept {
		return ref;
	}



	GLTextureDataReceiver::GLTextureDataReceiver(uint32_t pix, glm::ivec2 _size, TextureFormat form)
		: pixelBuffer(pix)
		, size(_size)
		, format(form)
		, sync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0))
	{}
	GLTextureDataReceiver::GLTextureDataReceiver(GLTextureDataReceiver&& other) noexcept
		: pixelBuffer(other.pixelBuffer)
		, size(other.size)
		, format(other.format)
		, sync(std::move(other.sync))
	{
		other.pixelBuffer = 0;
	}
	GLTextureDataReceiver::~GLTextureDataReceiver() {
		if (pixelBuffer != 0) {
			glDeleteBuffers(1, &pixelBuffer);
			pixelBuffer = 0;
		}
	}
	GLTextureDataReceiver& GLTextureDataReceiver::operator=(GLTextureDataReceiver&& other) noexcept {
		GLTextureDataReceiver::~GLTextureDataReceiver();
		pixelBuffer = other.pixelBuffer;
		size = other.size;
		format = other.format;
		sync = std::move(other.sync);
		other.pixelBuffer = 0;
		return *this;
	}

	GLBufferDataReceiver::GLBufferDataReceiver(const std::shared_ptr<GLBufferObject>& obj, BufferTarget tar, std::size_t b, std::size_t e)
		: object(obj)
		, target(tar)
		, begin(b)
		, end(e)
		, sync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0))
	{}
	GLBufferDataReceiver::GLBufferDataReceiver(GLBufferDataReceiver&& other) noexcept
		: object(std::move(other.object))
		, target(other.target)
		, begin(other.begin)
		, end(other.end)
		, sync(std::move(other.sync))
	{}
	GLBufferDataReceiver::~GLBufferDataReceiver() {}

	GLBufferDataReceiver& GLBufferDataReceiver::operator=(GLBufferDataReceiver&& other) noexcept {
		GLBufferDataReceiver::~GLBufferDataReceiver();
		object = std::move(other.object);
		target = other.target;
		begin = other.begin;
		end = other.end;
		new (&sync) GLFence(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
		return *this;
	}
};