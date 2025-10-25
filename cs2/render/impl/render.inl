#pragma once

#include "../render.hpp"

ALWAYS_INLINE const auto& c_render::device( ) const { return m_device; }

ALWAYS_INLINE const auto& c_render::device_ctx( ) const { return m_device_ctx; }

ALWAYS_INLINE auto& c_render::render_view( ) { return m_render_view; }