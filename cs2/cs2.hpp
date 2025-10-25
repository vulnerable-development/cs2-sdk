#pragma once

#include <sdk.hpp>

#define OFFSET( type, definition, offset )                                          \
    ALWAYS_INLINE auto& definition {                                                \
        return *reinterpret_cast< std::add_pointer_t< type > >(                     \
            reinterpret_cast< std::uintptr_t >( this ) + offset                     \
        );                                                                          \
    }                                                                               \

#define VFUNC( type, definition, index, ... )                                       \
    ALWAYS_INLINE auto definition {                                                 \
        return reinterpret_cast< type >(                                            \
            ( *reinterpret_cast< std::uintptr_t** >( this ) )[ index ]              \
        )( this, __VA_ARGS__ );                                                     \
    }                                                                               \

#define OFFSET_VFUNC( type, definition, offset, ... )                               \
    ALWAYS_INLINE auto definition {                                                 \
        return offset.as< type >( )( this, __VA_ARGS__ );                           \
    }                                                                               \

#define POFFSET_VFUNC( type, definition, offset, ... )                              \
    ALWAYS_INLINE auto definition {                                                 \
        return offset.as< type >( )( __VA_ARGS__ );                                 \
    }                                                                               \


#include "ctx/ctx.hpp"
#include "menu/menu.hpp"

#include "render/render.hpp"

#include "valve/valve.hpp"
#include "local_player/local_player.hpp"

#include "hacks/hacks.hpp"
#include "hooks/hooks.hpp"