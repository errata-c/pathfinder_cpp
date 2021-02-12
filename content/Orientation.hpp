#pragma once

namespace pf {
    struct Outline;

    enum class Orientation {
        /// Counterclockwise, with Y down.
        ccw = -1,
        /// Clockwise, with Y down.
        cw = 1,
    };

    Orientation orientationOf(const Outline&);
};