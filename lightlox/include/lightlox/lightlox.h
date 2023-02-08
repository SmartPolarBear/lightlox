//
// Created by bear on 2/8/2023.
//

#pragma once

namespace lightlox
{
static inline constexpr bool DEBUG = true;
}

#define DEBUG_ONLY if constexpr (DEBUG)
