#pragma once
#include <cinttypes>
#include <vector>
#include <memory>
#include <string_view>
#include <string>
#include <optional>
#include <chrono>


#include <glm/vec2.hpp>

#include "../gpu/Enums.hpp"
#include "../gpu/GPU.hpp"

#include "Types.hpp"

namespace pf {
	using Duration = std::chrono::duration<uint64_t, std::nano>;

	enum class GLVersion {
		gl3,
		gles3,
		gl4,
	};

	struct GLDevice {
		using Buffer = GLBuffer;
		using Fence = GLFence;
		using Framebuffer = GLFramebuffer;
		using ImageParameter = GLImageParam;
		using Program = GLProgram;
		using Shader = GLShader;
		using StorageBuffer = GLStorageBuffer;
		using Texture = GLTexture;
		using TextureParameter = GLTextureParam;
		using Uniform = GLUniform;
		using VertexArray = GLVertexArray;
		using VertexAttr = GLVertexAttr;
		using TimerQuery = GLTimerQuery;
		using TextureDataReceiver = GLTextureDataReceiver;
		using BufferDataReceiver = GLBufferDataReceiver;

		GLDevice(GLVersion ver, uint32_t fb);

		void setRenderState(const RenderState<GLDevice>& state);
		void setComputeState(const ComputeState<GLDevice>& state);
		void resetRenderState(const RenderState<GLDevice>& state);
		void resetComputeState(const ComputeState<GLDevice>& state);

		void setDefaultFramebuffer(uint32_t fb);
		void bindTexturesAndImages(GLProgram & program);
		void setRenderOptions(const RenderOptions & options);
		void setUniform(const GLUniform& uniform, const UniformData& data);
		void setStorageBuffer(GLStorageBuffer& storage, GLBuffer & buffer);
		void unsetStorageBuffer(GLStorageBuffer& storage);
		void resetRenderOptions(const RenderOptions& options);

		void bindRenderTarget(const RenderTarget<GLDevice> & attach);
		void bindVertexArray(const GLVertexArray& arr);
		void unbindVertexArray();
		void bindTexture(const GLTexture & tex, uint32_t unit);
		void unbindTexture(uint32_t unit);
		//void bindImage()
		void unbindImage(uint32_t unit);
		void useProgram(const GLProgram & prog);
		void unuseProgram();
		void bindDefaultFramebuffer();
		void bindFramebuffer(const GLFramebuffer & fb);
		void clear(const ClearOps& ops);
		TextureFormat renderTargetFormat(const RenderTarget<GLDevice> & target);
		TextureData getTextureData(const GLTextureDataReceiver& receiver);
		std::vector<uint8_t> getBufferData(const GLBufferDataReceiver& receiver);


		// impl Device abstraction
		std::string_view backendName() const;
		std::string deviceName() const;

		FeatureLevel featureLevel() const;

		GLTexture createTexture(TextureFormat format, glm::ivec2 size);
		GLTexture createTexture(TextureFormat format, glm::ivec2 size, const void* dataBegin, const void* dataEnd);
		GLShader createShader(std::string_view name, std::string_view source, ShaderKind kind);
		GLProgram createProgram(std::string_view name, pf::Program<GLShader>&& shaders);

		// Not needed for OpenGL
		void setComputeProgramLocalSize(Program&, ComputeDimensions) {};

		GLVertexArray createVertexArray();
		std::optional<GLVertexAttr> getVertexAttr(const GLProgram & program, std::string_view name);
		GLUniform getUniform(const GLProgram& program, std::string_view name);
		GLTextureParam getTextureParam(GLProgram& program, std::string_view name);
		GLImageParam getImageParam(const GLProgram& program, std::string_view name);
		GLStorageBuffer getStorageBuffer(const GLProgram& program, std::string_view name, uint32_t binding);
		void configureVertexAttr(const GLVertexArray& arr, const GLVertexAttr& attr, const VertexAttrDescriptor& desc);
		GLFramebuffer createFramebuffer(GLTexture && texture);
		GLBuffer createBuffer(BufferUploadMode mode);

		GLTexture& framebufferTexture(GLFramebuffer& fb);
		GLTexture destroyFramebuffer(GLFramebuffer&& framebuffer);
		TextureFormat textureFormat(const GLTexture& tex);
		glm::ivec2 textureSize(const GLTexture& tex);


		void beginCommands();
		void endCommands();


		void setTextureSamplingMode(const GLTexture & tex, TextureSamplingFlags flags);
		void uploadToTexture(const GLTexture & tex, RectI rect, const TextureData& ref);
		GLTextureDataReceiver readPixels(const RenderTarget<GLDevice> & target, RectI viewport);
		GLBufferDataReceiver readBuffer(const GLBuffer& buffer, BufferTarget target, std::size_t begin, std::size_t end);


		void drawArrays(uint32_t icount, const RenderState<GLDevice> & state);
		void drawElements(uint32_t icount, const RenderState<GLDevice> & state);
		void drawElementsInstanced(uint32_t icount, uint32_t instanceCount, const RenderState<GLDevice> & state);
		void dispatchCompute(ComputeDimensions dims, const ComputeState<GLDevice>& state);


		void bindbuffer(const GLVertexArray& varr, const GLBuffer& buff, BufferTarget target);
		GLFence addFence();
		void waitForFence(GLFence& fence);
		GLTimerQuery createTimerQuery();
		void beginTimerQuery(const GLTimerQuery & query);
		void endTimerQuery(const GLTimerQuery & query);


		std::optional<Duration> tryRecvTimerQuery(const GLTimerQuery& query);
		Duration recvTimerQuery(const GLTimerQuery& query);

		std::optional<std::vector<uint8_t>> tryRecvBuffer();
		std::vector<uint8_t> recvBuffer();

		TextureData recvTextureData(const TextureDataReceiver& receiver);
		std::optional<TextureData> tryRecvTextureData(const TextureDataReceiver& receiver);

		
		GLVersion version;
		uint32_t defaultFramebuffer;
		GLTexture dummyTexture;
	};

	
};