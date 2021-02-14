#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace pf {
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
        UniformData(const glm::vec2& val) noexcept;
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
}