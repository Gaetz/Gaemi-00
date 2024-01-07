//
// Created by gaetz on 08/04/2023.
//

#ifndef GAEMI_IM_RENDERER_H
#define GAEMI_IM_RENDERER_H

#include "raylib.h"
#include "Defines.hpp"

namespace gmath {
    class Rect;
}

using gmath::Rect;

namespace render::im {
    GAPI i32 GuiSlider(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
    GAPI i32 GuiSliderBar(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
    GAPI i32 GuiProgressBar(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
}

#endif //GAEMI_IM_RENDERER_H
