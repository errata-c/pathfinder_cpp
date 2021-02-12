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


    struct UniformData {
        enum class Kind {
            Float = 1,
            Vec2 = 2,
            Vec3 = 3,
            Vec4 = 4,

            Mat2 = 4 | (1 << 8),
            Mat3 = 9 | (1 << 8),
            Mat4 = 16 | (1 << 8),

            Int = 1 | (2 << 8),
            IVec2 = 2 | (2 << 8),
            IVec3 = 3 | (2 << 8),
        };
        static constexpr Kind
            Float = Kind::Float,
            Int = Kind::Int,
            IVec2 = Kind::IVec2,
            IVec3 = Kind::IVec3,
            Vec2 = Kind::Vec2,
            Vec3 = Kind::Vec3,
            Vec4 = Kind::Vec4,
            Mat2 = Kind::Mat2,
            Mat3 = Kind::Mat3,
            Mat4 = Kind::Mat4;

        UniformData() noexcept;
        UniformData(float val) noexcept;
        UniformData(int32_t val) noexcept;
        UniformData(const glm::vec2 & val) noexcept;
        UniformData(const glm::vec3& val) noexcept;
        UniformData(const glm::vec4& val) noexcept;
        UniformData(const glm::ivec2& val) noexcept;
        UniformData(const glm::ivec3& val) noexcept;
        UniformData(const glm::mat2& val) noexcept;
        UniformData(const glm::mat3& val) noexcept;
        UniformData(const glm::mat4& val) noexcept;
        ~UniformData();

        UniformData(const UniformData& other) noexcept;
        UniformData& operator=(const UniformData& other) noexcept;

        std::size_t byte_size() const noexcept;
        std::size_t size() const noexcept;

        float* data_f32();
        int32_t* data_i32();
        const float* data_f32() const;
        const int32_t* data_i32() const;

        float& asFloat();
        int32_t asInt();
        glm::mat2& asMat2();
        glm::mat3& asMat3();
        glm::mat4& asMat4();
        glm::vec2& asVec2();
        glm::vec3& asVec3();
        glm::vec4& asVec4();
        glm::ivec2& asIVec2();
        glm::ivec3& asIVec3();

        const float& asFloat() const;
        const int32_t asInt() const;
        const glm::mat2& asMat2() const;
        const glm::mat3& asMat3() const;
        const glm::mat4& asMat4() const;
        const glm::vec2& asVec2() const;
        const glm::vec3& asVec3() const;
        const glm::vec4& asVec4() const;
        const glm::ivec2& asIVec2() const;
        const glm::ivec3& asIVec3() const;

        const Kind kind() const noexcept;
    private:
        Kind discrim;
        union {
            float fdata[16];
            int32_t idata[16];
        };
    };

    using ComputeDimensions = glm::ivec3;

    struct TextureData {
        enum class Kind {
            U8,
            U16,
            F16,
            F32,
        };
        static constexpr Kind
            U8 = Kind::U8,
            U16 = Kind::U16,
            F16 = Kind::F16,
            F32 = Kind::F32;

        TextureData(Kind _kind);
        TextureData(const TextureData& other);
        TextureData(TextureData&& other) noexcept;
        ~TextureData();
        TextureData& operator=(const TextureData& other);
        TextureData& operator=(TextureData&& other) noexcept;
        
        bool empty() const noexcept;
        std::size_t max_size() const noexcept;
        std::size_t size() const noexcept;
        void clear();

        void resize(std::size_t cap);
        void reserve(std::size_t cap);
        std::size_t capacity() const noexcept;
        void shrink_to_fit();

        void swap(TextureData&& other) noexcept;

        std::vector<uint8_t>& asU8();
        std::vector<uint16_t>& asU16();
        std::vector<pf::half>& asF16();
        std::vector<float>& asF32();

        const std::vector<uint8_t>& asU8() const;
        const std::vector<uint16_t>& asU16() const;
        const std::vector<pf::half>& asF16() const;
        const std::vector<float>& asF32() const;

        const Kind kind() const noexcept;
    private:
        Kind discrim;
        union {
            std::vector<uint8_t> u8;
            std::vector<uint16_t> u16;
            std::vector<pf::half> f16;
            std::vector<float> f32;
        };
    };

    struct VertexAttrDescriptor {
        std::size_t size;
        VertexAttrClass vertClass;
        VertexAttrType attrType;
        std::size_t stride, offset;
        uint32_t divisor, bufferIndex;
    };

    template<typename T>
    struct UniformBinding {

        UniformData data;
    };

    template<typename TP, typename T>
    struct TextureBinding {


    };

    template<typename IP, typename T>
    struct ImageBinding {

        ImageAccess access;
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
};