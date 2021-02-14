#include "GPU.hpp"



namespace pf {
    RenderOptions::RenderOptions() noexcept
        : colorMask(true)
    {}

    StencilState::StencilState() noexcept
        : func(StencilFunc::Always)
        , reference(0)
        , mask(~0)
        , write(false)
    {}

    BlendState::BlendState() noexcept
        : srcRGBFactor(BlendFactor::One)
        , destRGBFactor(BlendFactor::OneMinusSrcAlpha)
        , srcAlphaFactor(BlendFactor::One)
        , destAlphaFactor(BlendFactor::One)
        , op(BlendOp::Add)
    {}

    bool ClearOps::empty() const noexcept {
        return !color && !depth && !stencil;
    }
};