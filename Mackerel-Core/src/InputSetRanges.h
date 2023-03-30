#pragma once

#include <cstdint>

/* 0 - 999 are keyboard inputs */
#define MKC_KEY_OFFSET 0
#define MCK_KEY_SPAN 1000

/* 1000 - 1999 are gamepad inputs */
#define MCK_GP_OFFSET (MKC_KEY_OFFSET + MCK_KEY_SPAN)
#define MCK_GP_SPAN 1000

/* 2000 - 2999 are mouse inputs */
#define MCK_MOUSE_OFFSET (MCK_GP_OFFSET + MCK_GP_SPAN)
#define MCK_MOUSE_SPAN 1000
