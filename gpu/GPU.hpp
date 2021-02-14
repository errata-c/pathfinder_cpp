#pragma once
#include <optional>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <color/color.hpp>
#include "Enums.hpp"
#include "half.hpp"

#include "TextureData.hpp"
#include "UniformData.hpp"

#include "Util.hpp"

#include "../geometry/Rect.hpp"


namespace pf {
    struct ClearOps {
        std::optional<ColorF> color;
        std::optional<float> depth;
        std::optional<uint8_t> stencil;

        bool empty() const noexcept;
    };

    struct BlendState {
        BlendState(const BlendState&) noexcept = default;
        BlendState& operator=(const BlendState&) noexcept = default;
        ~BlendState() = default;

        BlendState() noexcept;

        BlendFactor destRGBFactor;
        BlendFactor destAlphaFactor;
        BlendFactor srcRGBFactor;
        BlendFactor srcAlphaFactor;
        BlendOp op;
    };

    struct DepthState {
        DepthFunc func;
        bool write;
    };

    struct StencilState {
        StencilState(const StencilState&) noexcept = default;
        StencilState& operator=(const StencilState&) noexcept = default;
        ~StencilState() = default;

        StencilState() noexcept;

        StencilFunc func;
        uint32_t reference;
        uint32_t mask;
        bool write;
    };

    using ComputeDimensions = glm::ivec3;

    struct VertexAttrDescriptor {
        std::size_t size;
        VertexAttrClass vertClass;
        VertexAttrType attrType;
        std::size_t stride, offset;
        uint32_t divisor, bufferIndex;
    };

    template<typename U>
    struct UniformBinding {
        const U* uniform;
        UniformData data;
    };

    template<typename TP, typename T>
    struct TextureBinding {
        const TP* parameter;
        const T* texture;
    };

    template<typename IP, typename T>
    struct ImageBinding {
        const IP* parameter;
        const T* texture;
        ImageAccess access;
    };

    template<typename StorageBuffer, typename Buffer>
    struct StorageBinding {
        const StorageBuffer* storage;
        const Buffer* buffer;
    };

    struct RenderOptions {
        RenderOptions(const RenderOptions&) noexcept = default;
        RenderOptions& operator=(const RenderOptions&) noexcept = default;
        ~RenderOptions() = default;

        RenderOptions() noexcept;

        std::optional<BlendState> blend;
        std::optional<DepthState> depth;
        std::optional<StencilState> stencil;
        ClearOps clearOps;
        bool colorMask;
    };

    template<typename Shader>
    struct Program {
        Program(Shader&& vert, Shader&& frag) noexcept 
            : vertex(vert)
            , fragment(frag)
            , kind(ProgramKind::Raster)
        {}
        Program(Shader&& comp) noexcept
            : compute(comp)
        {}

        Program(Program&&) noexcept = default;
        Program& operator=(Program&&) noexcept = default;
        ~Program() = default;

        ProgramKind kind;
        Shader vertex, fragment, compute;
    };

    template<typename D>
    struct RenderState {
        const RenderTarget<D>* target;
        const typename D::Program * program;
        const typename D::VertexArray * vertexArray;
        Primitive primitive;
        
        Slice<UniformBinding<typename D::Uniform>> uniforms;
        Slice<TextureBinding<typename D::TextureParameter, typename D::Texture>> textures;
        Slice<ImageBinding<typename D::ImageParameter, typename D::Texture>> images;
        Slice<StorageBinding<typename D::StorageBuffer, typename D::Buffer>> storageBuffers;

        RectI viewport;
        RenderOptions options;
    };

    template<typename D>
    struct ComputeState {
        const typename D::Program * program;

        Slice<UniformBinding<typename D::Uniform>> uniforms;
        Slice<TextureBinding<typename D::TextureParameter, typename D::Texture>> textures;
        Slice<ImageBinding<typename D::ImageParameter, typename D::Texture>> images;
        Slice<StorageBinding<typename D::StorageBuffer, typename D::Buffer>> storageBuffers;
    };

    enum class RenderTargetKind {
        Default,
        Framebuffer
    };
    template<typename D>
    struct RenderTarget {
        RenderTargetKind kind;
        typename D::Framebuffer const * framebuffer;
    };
};