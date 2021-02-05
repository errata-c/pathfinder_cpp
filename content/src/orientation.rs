// pathfinder/geometry/src/orientation.rs
//
// Copyright © 2019 The Pathfinder Project Developers.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

//! Determining whether an outline is wound clockwise or counterclockwise.

use crate::outline::Outline;

/// Winding order: counterclockwise or clockwise, with Y down.
#[derive(Clone, Copy, Debug, PartialEq)]
pub enum Orientation {
    /// Counterclockwise, with Y down.
    Ccw = -1,
    /// Clockwise, with Y down.
    Cw = 1,
}

impl Orientation {
    /// Determines whether the outermost winding of an outline is counterclockwise or clockwise.
    ///
    /// This follows the FreeType algorithm.
    pub fn from_outline(outline: &Outline) -> Orientation {
        let mut area = 0.0;
        for contour in &outline.contours {
            let mut prev_position = match contour.last_position() {
                None => continue,
                Some(position) => position,
            };
            for &next_position in &contour.points {
                area += prev_position.det(next_position);
                prev_position = next_position;
            }
        }
        Orientation::from_area(area)
    }

    fn from_area(area: f32) -> Orientation {
        if area <= 0.0 {
            Orientation::Ccw
        } else {
            Orientation::Cw
        }
    }
}
