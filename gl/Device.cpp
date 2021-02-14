#include "Device.hpp"
#include <gl/glew.h>
#include <algorithm>
#include <string>
#include <cassert>
#include <fmt/format.h>

#include "Util.hpp"

static constexpr int dummyLength = 16;

static void ck() {
	pf::glCheckErrors();
}

static std::vector<uint8_t> preprocess(std::string_view source, std::string_view version) {
	char prior = '\0';
	std::vector<uint8_t> output;

	auto it = source.begin(), end = source.end();

	while(it != end) {
		char current = *it;
		if (current == '{' && prior == '{') {
			output.pop_back();
			while (*it != '}') {
				++it;
				assert(it != end);
			}
			assert(*it == '}');
			++it;

			std::copy(version.begin(), version.end(), std::back_inserter(output));
			prior = '\0';
		}
		else {
			output.push_back(current);
		}
		prior = current;
	}
	return output;
}

namespace pf {
	GLDevice::GLDevice(GLVersion ver, uint32_t fb)
		: dummyTexture(0)
		, version(ver)
		, defaultFramebuffer(fb)
	{
		int32_t dummyData[dummyLength];
		std::fill(dummyData, dummyData + dummyLength, 0);

		dummyTexture = createTexture(TextureFormat::RGBA8, glm::ivec2{ dummyLength }, dummyData, dummyData + dummyLength);
	}

	void GLDevice::setRenderState(const RenderState<GLDevice>& state) {
		bindRenderTarget(*state.target);

		glViewport(state.viewport.minX(), state.viewport.minY(), state.viewport.width(), state.viewport.height());

		if (!state.options.clearOps.empty()) {
			clear(state.options.clearOps);
		}

		useProgram(*state.program);
		bindVertexArray(*state.vertexArray);

		// TODO finish this.
	}
	void GLDevice::setComputeState(const ComputeState<GLDevice>& state) {

	}
	void GLDevice::resetRenderState(const RenderState<GLDevice>& state) {

	}
	void GLDevice::resetComputeState(const ComputeState<GLDevice>& state) {

	}

	void GLDevice::setDefaultFramebuffer(uint32_t fb) {
		defaultFramebuffer = fb;
	}
	void GLDevice::setRenderOptions(const RenderOptions& options) {
		if (options.blend) {
			const BlendState & blend = options.blend.value();
			glBlendFuncSeparate(
				convertGL(blend.srcRGBFactor),
				convertGL(blend.destRGBFactor),
				convertGL(blend.srcAlphaFactor),
				convertGL(blend.destAlphaFactor)
			);
			ck();
			glBlendEquation(convertGL(blend.op));
			ck();
			glEnable(GL_BLEND);
			ck();
		}
		else {
			glDisable(GL_BLEND);
			ck();
		}

		if (options.depth) {
			const DepthState& depth = options.depth.value();
			glDepthFunc(convertGL(depth.func));
			ck();
			glDepthMask(depth.write);
			ck();
			glEnable(GL_DEPTH);
			ck();
		}
		else {
			glDisable(GL_DEPTH);
			ck();
		}

		if (options.stencil) {
			const StencilState& stencil = options.stencil.value();
			glStencilFunc(
				convertGL(stencil.func),
				stencil.reference,
				stencil.mask
			); ck();

			int32_t passAction = GL_KEEP, writeMask = 0;
			if (stencil.write) {
				passAction = GL_REPLACE;
				writeMask = stencil.mask;
			}

			glStencilOp(GL_KEEP, GL_KEEP, passAction); ck();
			glStencilMask(writeMask);
			
			glEnable(GL_STENCIL); ck();
		}
		else {
			glDisable(GL_STENCIL); ck();
		}

		glColorMask(options.colorMask, options.colorMask, options.colorMask, options.colorMask); ck();
	}
	void GLDevice::setUniform(const GLUniform& uniform, const UniformData& data) {
		switch (data.kind()) {
		case UniformData::Float:
			glUniform1fv(uniform.id(), 1, data.data_f32()); ck();
			break;
		case UniformData::Vec2:
			glUniform2fv(uniform.id(), 1, data.data_f32()); ck();
			break;
		case UniformData::Vec3:
			glUniform3fv(uniform.id(), 1, data.data_f32()); ck();
			break;
		case UniformData::Vec4:
			glUniform4fv(uniform.id(), 1, data.data_f32()); ck();
			break;

		case UniformData::Mat2:
			glUniformMatrix2fv(uniform.id(), 1, GL_FALSE, data.data_f32()); ck();
			break;
		case UniformData::Mat3:
			glUniformMatrix3fv(uniform.id(), 1, GL_FALSE, data.data_f32()); ck();
			break;
		case UniformData::Mat4:
			glUniformMatrix4fv(uniform.id(), 1, GL_FALSE, data.data_f32()); ck();
			break;

		case UniformData::Int:
			glUniform1iv(uniform.id(), 1, data.data_i32()); ck();
			break;
		case UniformData::IVec2:
			glUniform2iv(uniform.id(), 1, data.data_i32()); ck();
			break;
		case UniformData::IVec3:
			glUniform3iv(uniform.id(), 1, data.data_i32()); ck();
			break;
		
		default:
			break;
		}
	}
	void GLDevice::setStorageBuffer(GLStorageBuffer& storage, GLBuffer& buffer) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage.location, buffer.object->id()); ck();
	}
	void GLDevice::unsetStorageBuffer(GLStorageBuffer& storage) {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage.location, 0);
	}

	void GLDevice::resetRenderOptions(const RenderOptions& options) {
		if (options.blend) {
			glDisable(GL_BLEND);
		}
		if (options.depth) {
			glDisable(GL_DEPTH); ck();
		}
		if (options.stencil) {
			glStencilMask(~0); ck();
			glDisable(GL_STENCIL_TEST);
		}

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); ck();
	}

	// impl Device abstraction
	std::string_view GLDevice::backendName() const {
		constexpr char name[] = "OpenGL";
		return std::string_view(name, sizeof(name) - 1);
	}
	std::string GLDevice::deviceName() const {
		const char* dev = (const char*)glGetString(GL_RENDERER);
		if (dev == nullptr) {
			ck();
			return {};
		}
		return std::string{ dev };
	}

	FeatureLevel GLDevice::featureLevel() const {
		switch (version) {
		case GLVersion::gl3:
		case GLVersion::gles3:
			return FeatureLevel::D3D9;
		case GLVersion::gl4:
			return FeatureLevel::D3D11;
		}
	}

	GLTexture GLDevice::createTexture(TextureFormat format, glm::ivec2 size) {
		uint32_t ref = 0;
		glGenTextures(1, &ref); ck();
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			glInternalFormat(format),
			size.x,
			size.y,
			0,
			glFormat(format),
			glType(format),
			nullptr
		);
		ck();

		GLTexture tex{ ref, size };
		setTextureSamplingMode(tex, TextureSamplingFlags::None);
		return std::move(tex);
	}
	GLTexture GLDevice::createTexture(TextureFormat format, glm::ivec2 size, const void* dataBegin, const void* dataEnd) {
#ifdef PF_DEBUG_ASSERTIONS
		switch (glType(format)) {
		case GL_UNSIGNED_BYTE: {
			std::size_t dataSize = (const uint8_t*)dataEnd - (const uint8_t*)dataBegin;
			std::size_t texArea = static_cast<std::size_t>(size.x) * size.y;

			assert(dataSize >= texArea);
			break;
		}
		case GL_HALF_FLOAT: {
			std::size_t dataSize = (const uint16_t*)dataEnd - (const uint16_t*)dataBegin;
			std::size_t texArea = static_cast<std::size_t>(size.x) * size.y;

			assert(dataSize >= texArea * sizeof(uint16_t));
			break;
		}
		case GL_FLOAT: {
			std::size_t dataSize = (const float*)dataEnd - (const float*)dataBegin;
			std::size_t texArea = static_cast<std::size_t>(size.x) * size.y;

			assert(dataSize >= texArea * sizeof(float));
			break;
		}
		}
#endif
		uint32_t ref = 0;
		glGenTextures(1, &ref); ck();

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			glInternalFormat(format),
			size.x,
			size.y,
			0,
			glFormat(format),
			glType(format),
			dataBegin
		);
		ck();

		GLTexture tex{ ref, size };
		setTextureSamplingMode(tex, TextureSamplingFlags::None);
		return std::move(tex);
	}
	GLShader GLDevice::createShader(std::string_view name, std::string_view source, ShaderKind kind) {
		std::string_view spec;
		switch (version) {
		case GLVersion::gl3:
			spec = "330";
			break;
		case GLVersion::gl4:
			spec = "430";
			break;
		case GLVersion::gles3:
			spec = "300 es";
			break;
		}

		std::vector<uint8_t> shaderSource = preprocess(source, spec);
		int32_t shaderKind = convertGL(kind);
		uint32_t shaderId = glCreateShader(shaderKind); ck();

		int32_t sourceLen = static_cast<int32_t>(shaderSource.size());
		const char* sourceData = (const char *)shaderSource.data();

		glShaderSource(shaderId, 1, &sourceData, &sourceLen); ck();
		glCompileShader(shaderId);
		
		int32_t compileStatus = 0;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus != GL_TRUE) {
			int32_t infoLen = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
			std::string infoLog(infoLen, 0);
			glGetShaderInfoLog(shaderId, infoLog.size(), nullptr, infoLog.data());
			fmt::print(stderr, "Compilation of OpenGL shader '{}' of kind '{}' failed!\n", kind, name);
			fmt::print(stderr, "Error log:\n{}", infoLog);
			assert(false);
		}

		return GLShader(shaderId);
	}
	GLProgram GLDevice::createProgram(std::string_view name, pf::Program<GLShader>&& shadersRef) {
		// Take ownership of the shaders. They will be deleted at the close of this scope.
		pf::Program<GLShader> shaders{ std::move(shadersRef) };

		uint32_t id = 0;
		id = glCreateProgram(); ck();
		switch (shaders.kind) {
		case ProgramKind::Raster:
			glAttachShader(id, shaders.vertex.id()); ck();
			glAttachShader(id, shaders.fragment.id()); ck();
			break;
		case ProgramKind::Compute:
			glAttachShader(id, shaders.compute.id()); ck();
			break;
		}
		glLinkProgram(id); ck();

		int32_t status = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &status); ck();
		if (status != GL_TRUE) {
			int32_t infoLen = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);
			std::string infoLog(infoLen, 0);

			glGetProgramInfoLog(id, infoLog.size(), nullptr, infoLog.data());
			fmt::print(stderr, "Linkage of OpenGL program '{}' of kind '{}' failed!\n", shaders.kind, name);
			fmt::print(stderr, "Error log:\n{}", infoLog);
			assert(false);
		}

		return GLProgram(id);
	}
	GLVertexArray GLDevice::createVertexArray() {
		uint32_t id = 0;
		glGenVertexArrays(1, &id); ck();
		return GLVertexArray(id);
	}
	std::optional<GLVertexAttr> GLDevice::getVertexAttr(const GLProgram& program, std::string_view name) {
		std::string name = fmt::format("a{}", name);
		int32_t attr = glGetAttribLocation(program.id(), name.data()); ck();
		if (attr < 0) {
			return std::nullopt;
		}
		else {
			return GLVertexAttr(attr);
		}
	}

	// TODO either add the prefix into the name before hand, or remove from shader sources, because this is dumb.
	GLUniform GLDevice::getUniform(const GLProgram& program, std::string_view name) {
		std::string name = fmt::format("u{}", name);
		uint32_t loc = glGetUniformLocation(program.id(), name.data()); ck();
		return GLUniform{ loc };
	}

	GLTextureParam GLDevice::getTextureParam(GLProgram& program, std::string_view name) {
		GLUniform uniform = getUniform(program, name);
		auto& params = program.params;
		uint32_t index = 0;
		for (; index < params->textures.size(); ++index) {
			if (params->textures[index] == uniform) {
				break;
			}
		}

		if (index == params->textures.size()) {
			params->textures.push_back(uniform);
		}
		return GLTextureParam{ uniform, index };
	}
	GLImageParam GLDevice::getImageParam(const GLProgram& program, std::string_view name) {
		GLUniform uniform = getUniform(program, name);
		auto& params = program.params;
		uint32_t index = 0;
		for (; index < params->images.size(); ++index) {
			if (params->images[index] == uniform) {
				break;
			}
		}

		if (index == params->images.size()) {
			params->images.push_back(uniform);
		}
		return GLImageParam{ uniform, index };
	}

	void GLDevice::configureVertexAttr(const GLVertexArray& arr, const GLVertexAttr& attr, const VertexAttrDescriptor& desc) {
		assert(desc.stride != 0);
		bindVertexArray(arr);

		int32_t attrType = convertGL(desc.attrType);
		switch (desc.vertClass) {
		case VertexAttrClass::Float:
		case VertexAttrClass::FloatNorm: {
			glVertexAttribPointer(
				attr.id(),
				desc.size,
				attrType,
				desc.vertClass == VertexAttrClass::FloatNorm ? GL_TRUE : GL_FALSE,
				desc.stride,
				(const void*)desc.offset
			); ck();
			break;
		}
		case VertexAttrClass::Int: {
			glVertexAttribIPointer(
				attr.id(),
				desc.size,
				attrType,
				desc.stride,
				(const void*)desc.offset
			); ck();
			break;
		}
		}

		glVertexAttribDivisor(attr.id(), desc.divisor); ck();
		glEnableVertexAttribArray(attr.id()); ck();

		unbindVertexArray();
	}

	GLStorageBuffer GLDevice::getStorageBuffer(const GLProgram& program, std::string_view name, uint32_t binding) {
		return GLStorageBuffer{ binding };
	}
	GLFramebuffer GLDevice::createFramebuffer(GLTexture&& texture) {
		uint32_t id = 0;
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		bindTexture(texture, 0);
		
	}
	GLBuffer GLDevice::createBuffer(BufferUploadMode mode) {
		uint32_t id = 0;
		glGenBuffers(1, &id);
		return GLBuffer{ std::shared_ptr<GLBufferObject>{new GLBufferObject(id)}, mode };
	}

	GLTexture& GLDevice::framebufferTexture(GLFramebuffer& fb) {
		return fb.texture;
	}
	GLTexture GLDevice::destroyFramebuffer(GLFramebuffer&& framebuffer) {
		GLFramebuffer fb = std::move(framebuffer);
		return std::move(fb.texture);
	}
	TextureFormat GLDevice::textureFormat(const GLTexture& tex) {
		return tex.format;
	}
	glm::ivec2 GLDevice::textureSize(const GLTexture& tex) {
		return tex.size;
	}

	void GLDevice::bindRenderTarget(const RenderTarget<GLDevice>& attach) {
		if (attach.kind == RenderTargetKind::Default) {
			bindDefaultFramebuffer();
		}
		else {
			bindFramebuffer(*attach.framebuffer);
		}
	}
	void GLDevice::bindVertexArray(const GLVertexArray& arr) {
		glBindVertexArray(arr.id()); ck();
	}
	void GLDevice::unbindVertexArray() {
		glBindVertexArray(0); ck();
	}
	void GLDevice::bindTexture(const GLTexture& tex, uint32_t unit) {
		glActiveTexture(GL_TEXTURE0 + unit); ck();
		glBindTexture(GL_TEXTURE_2D, tex.id()); ck();
	}
	void GLDevice::unbindTexture(uint32_t unit) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, 0); ck();
	}
	//void bindImage()
	void GLDevice::unbindImage(uint32_t unit) {
		glBindImageTexture(unit, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8); ck();
	}
	void GLDevice::useProgram(const GLProgram& prog) {
		glUseProgram(prog.id()); ck();
	}
	void GLDevice::unuseProgram() {
		glUseProgram(0); ck();
	}
	void GLDevice::bindDefaultFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer); ck();
	}
	void GLDevice::bindFramebuffer(const GLFramebuffer& fb) {
		glBindFramebuffer(GL_FRAMEBUFFER, fb.id()); ck();
	}
	void GLDevice::clear(const ClearOps& ops) {
		int32_t flags = 0;
		if (ops.color) {
			const pf::ColorF& color = ops.color.value();
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glClearColor(color.r, color.g, color.b, color.a);
			flags |= GL_COLOR_BUFFER_BIT;
		}
		if (ops.depth) {
			float depth = ops.depth.value();
			glDepthMask(GL_TRUE); ck();
			glClearDepthf(depth); ck();
			flags |= GL_DEPTH_BUFFER_BIT;
		}
		if (ops.stencil) {
			uint8_t stencil = ops.stencil.value();
			glStencilMask(~0); ck();
			glClearStencil(stencil); ck();
			flags |= GL_STENCIL_BUFFER_BIT;
		}
		if (flags != 0) {
			glClear(flags); ck();
		}
	}
	TextureFormat GLDevice::renderTargetFormat(const RenderTarget<GLDevice>& target) {
		if (target.kind == RenderTargetKind::Default) {
			return TextureFormat::RGBA8;
		}
		else {
			return target.framebuffer->texture.format;
		}
	}
	TextureData GLDevice::getTextureData(const GLTextureDataReceiver& receiver) {
		TextureFormat format = receiver.format;
		glm::ivec2 size = receiver.size;
		uint32_t chan = pf::channels(format);
		std::size_t area = size.x * static_cast<std::size_t>(size.y) * chan;
		std::size_t dataSize = area;
		void* dataPtr = nullptr;

		TextureData::Kind kind = TextureData::Kind::U8;

		switch (format) {
		case TextureFormat::R8:
		case TextureFormat::RGBA8:
			kind = TextureData::U8;
			break;
		case TextureFormat::R16F:
		case TextureFormat::RGBA16F:
			kind = TextureData::F16;
			dataSize *= 2;
			break;
		case TextureFormat::RGBA32F:
			kind = TextureData::F32;
			dataSize *= 4;
			break;
		}
		TextureData data(kind);
		data.resize(area);
		switch (data.kind()) {
		case TextureData::U8:
			dataPtr = data.asU8().data();
			break;
		case TextureData::F16:
			dataPtr = data.asF16().data();
			break;
		case TextureData::F32:
			dataPtr = data.asF32().data();
			break;
		}

		glBindBuffer(GL_PIXEL_PACK_BUFFER, receiver.pixelBuffer); ck();
		glGetBufferSubData(GL_PIXEL_PACK_BUFFER, 0, dataSize, dataPtr); ck();
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0); ck();

		// TODO flip y?

		return data;
	}
	std::vector<uint8_t> GLDevice::getBufferData(const GLBufferDataReceiver& receiver) {
		std::vector<uint8_t> dest(receiver.end - receiver.begin, 0);
		int32_t glTarget = convertGL(receiver.target);
		glBindBuffer(glTarget, receiver.object->id()); ck();
		glGetBufferSubData(glTarget, receiver.object->id(), receiver.end - receiver.begin, dest.data()); ck();
		return dest;
	}

	void GLDevice::beginCommands() {
		// nothing to do
	}
	void GLDevice::endCommands() {
		glFlush();
	}
	void GLDevice::setTextureSamplingMode(const GLTexture& tex, TextureSamplingFlags flags) {
		bindTexture(tex, 0);
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			pf::contains(flags, pf::TextureSamplingFlags::NearestMin) ? GL_NEAREST : GL_LINEAR
		); ck();
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			pf::contains(flags, pf::TextureSamplingFlags::NearestMag) ? GL_NEAREST : GL_LINEAR
		); ck();
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			pf::contains(flags, pf::TextureSamplingFlags::RepeatU) ? GL_REPEAT : GL_CLAMP_TO_EDGE
		); ck();
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			pf::contains(flags, pf::TextureSamplingFlags::RepeatV) ? GL_REPEAT : GL_CLAMP_TO_EDGE
		); ck();
	}

	void GLDevice::uploadToTexture(const GLTexture& tex, RectI rect, const TextureData& ref) {
		assert(rect.size().x >= 0);
		assert(rect.size().y >= 0);
		assert(rect.maxX() <= tex.size.x);
		assert(rect.maxY() <= tex.size.y);

		const void* data = ref.dataChecked(rect.size(), tex.format);

		bindTexture(tex, 0);
		if (rect.origin() == glm::ivec2{0} && rect.size() == tex.size) {
			glTexImage2D(GL_TEXTURE_2D,
				0,
				glInternalFormat(tex.format),
				tex.size.x,
				tex.size.y,
				0,
				glFormat(tex.format),
				glType(tex.format),
				data); ck();
		}
		else {
			glTexSubImage2D(GL_TEXTURE_2D,
				0,
				rect.minX(),
				rect.minY(),
				rect.maxX() - rect.minX(),
				rect.maxY() - rect.minY(),
				glFormat(tex.format),
				glType(tex.format),
				data); ck();
		}
		
		setTextureSamplingMode(tex, TextureSamplingFlags::None);
	}

	GLTextureDataReceiver GLDevice::readPixels(const RenderTarget<GLDevice>& target, RectI viewport) {
		glm::ivec2 origin = viewport.origin();
		glm::ivec2 size = viewport.size();
		TextureFormat format = renderTargetFormat(target);
		bindRenderTarget(target);
		std::size_t sizeBytes = std::size_t(size.x) * size.y * bytesPerPixel(format);

		uint32_t pixelbuffer = 0;
		glGenBuffers(1, &pixelbuffer); ck();
		glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelbuffer); ck();
		glBufferData(GL_PIXEL_PACK_BUFFER, sizeBytes, nullptr, GL_STATIC_READ); ck();

		glReadPixels(origin.x, origin.y, size.x, size.y, glFormat(format), glType(format), nullptr);

		return GLTextureDataReceiver(pixelbuffer, size, format);
	}
	GLBufferDataReceiver GLDevice::readBuffer(const GLBuffer& buffer, BufferTarget target, std::size_t begin, std::size_t end) {
		return GLBufferDataReceiver(buffer.object, target, begin, end);
	}

	void GLDevice::drawArrays(uint32_t icount, const RenderState<GLDevice>& state) {
		setRenderState(state);
		glDrawArrays(convertGL(state.primitive), 0, icount); ck();
		resetRenderState(state);
	}
	void GLDevice::drawElements(uint32_t icount, const RenderState<GLDevice>& state) {
		setRenderState(state);
		glDrawElements(convertGL(state.primitive), icount, GL_UNSIGNED_INT, nullptr); ck();
		resetRenderState(state);
	}
	void GLDevice::drawElementsInstanced(uint32_t icount, uint32_t instanceCount, const RenderState<GLDevice>& state) {
		setRenderState(state);
		glDrawElementsInstanced(convertGL(state.primitive), icount, GL_UNSIGNED_INT, nullptr, instanceCount); ck();
		resetRenderState(state);
	}
	void GLDevice::dispatchCompute(ComputeDimensions dims, const ComputeState<GLDevice>& state) {
		setComputeState(state);
		glDispatchCompute(dims.x, dims.y, dims.z); ck();
		resetComputeState(state);
	}
};