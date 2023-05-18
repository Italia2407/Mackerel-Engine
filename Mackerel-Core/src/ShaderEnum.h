#pragma once
enum class ShaderEnum
{
    __LIGHT_UNLIT_SHADOWS = -5,
    __LIGHT_UNLIT = -4,
    __FRAG_MONOCOLOUR = -3,
    __MCK_FRAMEBUFFER_DISPLAY = -2,
    __MCK_DEFAULT = -1,
    // Negative values are reserved for engine use
    monoColour = 0,
    unlitShadows = 1,
    fragmentHeader = 2,
    unlit = 3,
    lightsHeader = 4,
    depthOnly = 5,
    FBDisplayer = 6,
    projection = 7,
    skinned_projection = 8,
    vertexHeader = 9
};