#include "GPU.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace pf {
    UniformData::UniformData() noexcept
        : discrim(Kind::Float)
        , fdata{0.f}
    {}
    UniformData::UniformData(float val) noexcept
        : discrim(Kind::Float)
        , fdata{ val }
    {}
    UniformData::UniformData(int32_t val) noexcept
        : discrim(Kind::Int)
        , idata{ val }
    {}
    UniformData::UniformData(const glm::vec2& val) noexcept
        : discrim(Kind::Vec2)
        , fdata{ val.x, val.y }
    {}
    UniformData::UniformData(const glm::vec3& val) noexcept
        : discrim(Kind::Vec3)
        , fdata{ val.x, val.y, val.z }
    {}
    UniformData::UniformData(const glm::vec4& val) noexcept
        : discrim(Kind::Vec4)
        , fdata{ val.x, val.y, val.z, val.w }
    {}
    UniformData::UniformData(const glm::ivec2& val) noexcept
        : discrim(Kind::IVec2)
        , idata{ val.x, val.y }
    {}
    UniformData::UniformData(const glm::ivec3& val) noexcept
        : discrim(Kind::IVec3)
        , idata{ val.x, val.y, val.z }
    {}
    UniformData::UniformData(const glm::mat2& val) noexcept
        : discrim(Kind::Mat2)
    {
        const float* ptr = glm::value_ptr(val);
        for (int i = 0; i < 4; ++i) {
            fdata[i] = ptr[i];
        }
    }
    UniformData::UniformData(const glm::mat3& val) noexcept
        : discrim(Kind::Mat3)
    {
        const float* ptr = glm::value_ptr(val);
        for (int i = 0; i < 9; ++i) {
            fdata[i] = ptr[i];
        }
    }
    UniformData::UniformData(const glm::mat4& val) noexcept
        : discrim(Kind::Mat4)
    {
        const float* ptr = glm::value_ptr(val);
        for (int i = 0; i < 16; ++i) {
            fdata[i] = ptr[i];
        }
    }
    UniformData::~UniformData()
    {}

    UniformData::UniformData(const UniformData& other) noexcept 
        : discrim(other.discrim)
    {
        for (int i = 0; i < other.size(); ++i) {
            idata[i] = other.idata[i];
        }
    }
    UniformData& UniformData::operator=(const UniformData& other) noexcept {
        discrim = other.discrim;
        for (int i = 0; i < other.size(); ++i) {
            idata[i] = other.idata[i];
        }
        return *this;
    }

    std::size_t UniformData::byte_size() const noexcept {
        return size() * 4;
    }
    std::size_t UniformData::size() const noexcept {
       return static_cast<std::size_t>(discrim) & 0xFFull;
    }

    float* UniformData::data_f32() {
        assert((static_cast<int>(discrim) & 0x300) != (2 << 8));
        return fdata;
    }
    int32_t* UniformData::data_i32() {
        assert((static_cast<int>(discrim) & 0x300) == (2 << 8));
        return idata;
    }
    const float* UniformData::data_f32() const {
        assert((static_cast<int>(discrim) & 0x300) != (2 << 8));
        return fdata;
    }
    const int32_t* UniformData::data_i32() const {
        assert((static_cast<int>(discrim) & 0x300) == (2 << 8));
        return idata;
    }

    float& UniformData::asFloat() {
        assert(discrim == Kind::Float);
        return fdata[0];
    }
    int32_t UniformData::asInt() {
        assert(discrim == Kind::Int);
        return idata[0];
    }
    glm::mat2& UniformData::asMat2() {
        assert(discrim == Kind::Mat2);
        return reinterpret_cast<glm::mat2&>(*fdata);
    }
    glm::mat3& UniformData::asMat3() {
        assert(discrim == Kind::Mat3);
        return reinterpret_cast<glm::mat3&>(*fdata);
    }
    glm::mat4& UniformData::asMat4() {
        assert(discrim == Kind::Mat4);
        return reinterpret_cast<glm::mat4&>(*fdata);
    }
    glm::vec2& UniformData::asVec2() {
        assert(discrim == Kind::Vec2);
        return reinterpret_cast<glm::vec2&>(*fdata);
    }
    glm::vec3& UniformData::asVec3() {
        assert(discrim == Kind::Vec3);
        return reinterpret_cast<glm::vec3&>(*fdata);
    }
    glm::vec4& UniformData::asVec4() {
        assert(discrim == Kind::Vec4);
        return reinterpret_cast<glm::vec4&>(*fdata);
    }
    glm::ivec2& UniformData::asIVec2() {
        assert(discrim == Kind::IVec2);
        return reinterpret_cast<glm::ivec2&>(*idata);
    }
    glm::ivec3& UniformData::asIVec3() {
        assert(discrim == Kind::IVec3);
        return reinterpret_cast<glm::ivec3&>(*idata);
    }

    const float& UniformData::asFloat() const {
        assert(discrim == Kind::Float);
        return fdata[0];
    }
    const int32_t UniformData::asInt() const {
        assert(discrim == Kind::Int);
        return idata[0];
    }
    const glm::mat2& UniformData::asMat2() const {
        assert(discrim == Kind::Mat2);
        return reinterpret_cast<const glm::mat2&>(*fdata);
    }
    const glm::mat3& UniformData::asMat3() const {
        assert(discrim == Kind::Mat3);
        return reinterpret_cast<const glm::mat3&>(*fdata);
    }
    const glm::mat4& UniformData::asMat4() const {
        assert(discrim == Kind::Mat4);
        return reinterpret_cast<const glm::mat4&>(*fdata);
    }
    const glm::vec2& UniformData::asVec2() const {
        assert(discrim == Kind::Vec2);
        return reinterpret_cast<const glm::vec2&>(*fdata);
    }
    const glm::vec3& UniformData::asVec3() const {
        assert(discrim == Kind::Vec3);
        return reinterpret_cast<const glm::vec3&>(*fdata);
    }
    const glm::vec4& UniformData::asVec4() const {
        assert(discrim == Kind::Vec4);
        return reinterpret_cast<const glm::vec4&>(*fdata);
    }
    const glm::ivec2& UniformData::asIVec2() const {
        assert(discrim == Kind::IVec2);
        return reinterpret_cast<const glm::ivec2&>(*idata);
    }
    const glm::ivec3& UniformData::asIVec3() const {
        assert(discrim == Kind::IVec3);
        return reinterpret_cast<const glm::ivec3&>(*idata);
    }

    const UniformData::Kind UniformData::kind() const noexcept {
        return discrim;
    }

};