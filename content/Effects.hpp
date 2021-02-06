#pragma once
#include <optional>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "../color/color.hpp"
#include "../color/matrix.hpp"
#include "../geometry/LineSegment.hpp"

namespace pf {
    enum class BlurDirection {
        /// The horizontal axis.
        x,
        /// The vertical axis.
        y,
    };

    enum class BlendMode {
        // Porter-Duff, supported by GPU blender
        /// No regions are enabled.
        Clear,
        /// Only the source will be present.
        Copy,
        /// The source that overlaps the destination, replaces the destination.
        SrcIn,
        /// Source is placed, where it falls outside of the destination.
        SrcOut,
        /// Source is placed over the destination.
        SrcOver,
        /// Source which overlaps the destination, replaces the destination. Destination is placed
        /// elsewhere.
        SrcAtop,
        /// Destination which overlaps the source, replaces the source.
        DestIn,
        /// Destination is placed, where it falls outside of the source.
        DestOut,
        /// Destination is placed over the source.
        DestOver,
        /// Destination which overlaps the source replaces the source. Source is placed elsewhere.
        DestAtop,
        /// The non-overlapping regions of source and destination are combined.
        Xor,
        /// Display the sum of the source image and destination image. It is defined in the Porter-Duff
        /// paper as the plus operator.
        Lighter,

        // Others, unsupported by GPU blender
        /// Selects the darker of the backdrop and source colors.
        Darken,
        /// Selects the lighter of the backdrop and source colors.
        Lighten,
        /// The source color is multiplied by the destination color and replaces the destination.
        Multiply,
        /// Multiplies the complements of the backdrop and source color values, then complements the
        /// result.
        Screen,
        /// Multiplies or screens the colors, depending on the source color value. The effect is
        /// similar to shining a harsh spotlight on the backdrop.
        HardLight,
        /// Multiplies or screens the colors, depending on the backdrop color value.
        Overlay,
        /// Brightens the backdrop color to reflect the source color.
        ColorDodge,
        /// Darkens the backdrop color to reflect the source color.
        ColorBurn,
        /// Darkens or lightens the colors, depending on the source color value. The effect is similar
        /// to shining a diffused spotlight on the backdrop.
        SoftLight,
        /// Subtracts the darker of the two constituent colors from the lighter color.
        Difference,
        /// Produces an effect similar to that of the Difference mode but lower in contrast.
        Exclusion,
        /// Creates a color with the hue of the source color and the saturation and luminosity of the
        /// backdrop color.
        Hue,
        /// Creates a color with the saturation of the source color and the hue and luminosity of the
        /// backdrop color.
        Saturation,
        /// Creates a color with the hue and saturation of the source color and the luminosity of the
        /// backdrop color.
        Color,
        /// Creates a color with the luminosity of the source color and the hue and saturation of the
        /// backdrop color. This produces an inverse effect to that of the Color mode.
        Luminosity,

        Default = SrcOver,
    };

    bool occludesBackdrop(BlendMode mode) noexcept {
        switch (mode) {
        case BlendMode::SrcOver:
        case BlendMode::Clear:
            return true;
        default:
            return false;
        }
    };

    bool isDestructive(BlendMode mode) noexcept {
        switch (mode) {
        case BlendMode::Clear:
        case BlendMode::Copy:
        case BlendMode::SrcIn:
        case BlendMode::DestIn:
        case BlendMode::SrcOut:
        case BlendMode::DestAtop:
            return true;
        default:
            return false;
        }
    };

    static constexpr glm::vec4
        DefringingKernalCoreGraphics{ 0.033165660, 0.102074051, 0.221434336, 0.286651906 },
        DefringingKernalFreeType{ 0.0, 0.031372549, 0.301960784, 0.337254902 };

    static constexpr glm::vec2
        StemDarkeningFactors{ 0.0121, 0.0121 * 1.25 },
        MaxStemDarkeningAmount{ 0.3, 0.3 };

    static constexpr float MaxStemDarkeningPixelsPerEm = 72.0;


    struct PatternFilter {
        enum class Kind {
            Text,
            Blur,
            Color,
        };
        static constexpr Kind
            Text = Kind::Text,
            Blur = Kind::Blur,
            Color = Kind::Color;

        PatternFilter() noexcept;
        ~PatternFilter();
        PatternFilter(const PatternFilter & other) noexcept;
        PatternFilter& operator=(const PatternFilter& other) noexcept;
        



        Kind kind;

// Expand the union when debugging, easier to debug
#ifndef NDEBUG
        union {
#endif
            struct TextData {
                ColorF fgColor, bgColor;
                std::optional<glm::vec4> defringingKernal;
                bool gammaCorrection;
            } text;

            struct BlurData {
                BlurDirection direction;
                float sigma;
            };

            ColorMatrix colorMatrix;

#ifndef NDEBUG
        };
#endif

    };

    struct Filter {
        enum class Kind {
            None,
            RadialGradient,
            Pattern
        };
        static constexpr Kind
            None = Kind::None,
            RadialGradient = Kind::RadialGradient,
            Pattern = Kind::Pattern;

        Kind kind;

#ifndef NDEBUG
        union {
#endif

            struct RadialData {
                LineSegment2F line;
                glm::vec2 radii, uvOrigin;
            } radial;

            PatternFilter pattern;

#ifndef NDEBUG
        };
#endif

    };
    
    
};