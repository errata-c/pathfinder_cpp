#include "GPU.hpp"
#include <new>
#include <cassert>

namespace pf {
    TextureData::TextureData(Kind _kind)
        : discrim(_kind)
    {
        switch (kind()) {
        case U8:
            new (&u8) std::vector<uint8_t>{};
            break;
        case U16:
            new (&u16) std::vector<uint16_t>{};
            break;
        case F16:
            new (&f16) std::vector<pf::half>{};
            break;
        case F32:
            new (&f32) std::vector<float>{};
            break;
        default:
            break;
        }
    }
    TextureData::TextureData(const TextureData& other)
        : discrim(other.discrim)
    {
        switch (kind()) {
        case U8:
            new (&u8) std::vector<uint8_t>{other.u8};
            break;
        case U16:
            new (&u16) std::vector<uint16_t>{other.u16};
            break;
        case F16:
            new (&f16) std::vector<pf::half>{other.f16};
            break;
        case F32:
            new (&f32) std::vector<float>{other.f32};
            break;
        default:
            break;
        }
    }
    TextureData::TextureData(TextureData&& other) noexcept
        : discrim(other.discrim)
    {
        switch (kind()) {
        case U8:
            new (&u8) std::vector<uint8_t>{std::move(other.u8)};
            break;
        case U16:
            new (&u16) std::vector<uint16_t>{std::move(other.u16)};
            break;
        case F16:
            new (&f16) std::vector<pf::half>{std::move(other.f16)};
            break;
        case F32:
            new (&f32) std::vector<float>{std::move(other.f32)};
            break;
        default:
            break;
        }
    }
    TextureData::~TextureData() {
        switch (kind()) {
        case U8:
            u8.~vector();
            break;
        case U16:
            u16.~vector();
            break;
        case F16:
            f16.~vector();
            break;
        case F32:
            f32.~vector();
            break;
        default:
            break;
        }
    }
    TextureData& TextureData::operator=(const TextureData& other) {
        TextureData::~TextureData();
        discrim = other.discrim;
        switch (kind()) {
        case U8:
            u8 = other.u8;
            break;
        case U16:
            u16 = other.u16;
            break;
        case F16:
            f16 = other.f16;
            break;
        case F32:
            f32 = other.f32;
            break;
        default:
            break;
        }
        return *this;
    }
    TextureData& TextureData::operator=(TextureData&& other) noexcept {
        TextureData::~TextureData();
        discrim = other.discrim;
        switch (kind()) {
        case U8:
            u8 = std::move(other.u8);
            break;
        case U16:
            u16 = std::move(other.u16);
            break;
        case F16:
            f16 = std::move(other.f16);
            break;
        case F32:
            f32 = std::move(other.f32);
            break;
        default:
            break;
        }
        return *this;
    }

    bool TextureData::empty() const noexcept {
        switch (kind()) {
        case U8:
            return u8.empty();
        case U16:
            return u16.empty();
        case F16:
            return f16.empty();
        case F32:
            return f32.empty();
        default:
            return true;
        }
    }
    std::size_t TextureData::max_size() const noexcept {
        switch (kind()) {
        case U8:
            return u8.max_size();
        case U16:
            return u16.max_size();
        case F16:
            return f16.max_size();
        case F32:
            return f32.max_size();
        default:
            return 0;
        }
    }
    std::size_t TextureData::size() const noexcept {
        switch (kind()) {
        case U8:
            return u8.size();
        case U16:
            return u16.size();
        case F16:
            return f16.size();
        case F32:
            return f32.size();
        default:
            return 0;
        }
    }
    void TextureData::clear() {
        switch (kind()) {
        case U8:
            return u8.clear();
        case U16:
            return u16.clear();
        case F16:
            return f16.clear();
        case F32:
            return f32.clear();
        default:
            break;
        }
    }

    void TextureData::resize(std::size_t cap) {
        switch (kind()) {
        case U8:
            return u8.resize(cap);
        case U16:
            return u16.resize(cap);
        case F16:
            return f16.resize(cap);
        case F32:
            return f32.resize(cap);
        default:
            break;
        }
    }
    void TextureData::reserve(std::size_t cap) {
        switch (kind()) {
        case U8:
            return u8.reserve(cap);
        case U16:
            return u16.reserve(cap);
        case F16:
            return f16.reserve(cap);
        case F32:
            return f32.reserve(cap);
        default:
            break;
        }
    }
    std::size_t TextureData::capacity() const noexcept {
        switch (kind()) {
        case U8:
            return u8.capacity();
        case U16:
            return u16.capacity();
        case F16:
            return f16.capacity();
        case F32:
            return f32.capacity();
        default:
            return 0;
        }
    }
    void TextureData::shrink_to_fit() {
        switch (kind()) {
        case U8:
            return u8.shrink_to_fit();
        case U16:
            return u16.shrink_to_fit();
        case F16:
            return f16.shrink_to_fit();
        case F32:
            return f32.shrink_to_fit();
        default:
            break;
        }
    }

    std::vector<uint8_t>& TextureData::asU8() {
        assert(kind() == U8);
        return u8;
    }
    std::vector<uint16_t>& TextureData::asU16() {
        assert(kind() == U16);
        return u16;
    }
    std::vector<pf::half>& TextureData::asF16() {
        assert(kind() == F16);
        return f16;
    }
    std::vector<float>& TextureData::asF32() {
        assert(kind() == F32);
        return f32;
    }

    const std::vector<uint8_t>& TextureData::asU8() const {
        assert(kind() == U8);
        return u8;
    }
    const std::vector<uint16_t>& TextureData::asU16() const {
        assert(kind() == U16);
        return u16;
    }
    const std::vector<pf::half>& TextureData::asF16() const {
        assert(kind() == F16);
        return f16;
    }
    const std::vector<float>& TextureData::asF32() const {
        assert(kind() == F32);
        return f32;
    }

    const TextureData::Kind TextureData::kind() const noexcept {
        return discrim;
    }

    const void* TextureData::data() const {
        switch (kind()) {
        case U8:
            return u8.data();
        case U16:
            return u16.data();
        case F16:
            return f16.data();
        case F32:
            return f32.data();
        }
    }
    const void* TextureData::dataChecked(glm::ivec2 size, TextureFormat format) const {
        int chan = 0;
        switch (format) {
        case TextureFormat::R8:
            chan = 1;
            assert(kind() == U8);
            break;
        case TextureFormat::RGBA8:
            chan = 4;
            assert(kind() == U8);
            break;
        case TextureFormat::RGBA16F:
            chan = 4;
            assert(kind() == F16);
            break;
        case TextureFormat::RGBA32F:
            chan = 4;
            assert(kind() == F32);
            break;
        default:
            assert(false && "Unimplemented texture format!");
            break;
        }

        std::size_t area = std::size_t(size.x) * size.y;
        switch (kind()) {
        case U8:
            assert(u8.size() >= area * chan);
            return u8.data();
        case F16:
            assert(f16.size() >= area * chan);
            return f16.data();
        case F32:
            assert(f32.size() >= area * chan);
            return f32.data();
        default:
            assert(false);
            return nullptr;
        }
    }
};