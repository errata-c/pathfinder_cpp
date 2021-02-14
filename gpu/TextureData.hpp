#pragma once
#include <cinttypes>
#include <glm/vec2.hpp>
#include "half.hpp"

namespace pf {
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

        std::vector<uint8_t>& asU8();
        std::vector<uint16_t>& asU16();
        std::vector<pf::half>& asF16();
        std::vector<float>& asF32();

        const std::vector<uint8_t>& asU8() const;
        const std::vector<uint16_t>& asU16() const;
        const std::vector<pf::half>& asF16() const;
        const std::vector<float>& asF32() const;

        const Kind kind() const noexcept;

        const void* data() const;
        const void* dataChecked(glm::ivec2 size, TextureFormat format) const;
    private:
        Kind discrim;
        union {
            std::vector<uint8_t> u8;
            std::vector<uint16_t> u16;
            std::vector<pf::half> f16;
            std::vector<float> f32;
        };
    };
}