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
    textured = 1,
    unlitShadows = 2,
    fragmentHeader = 3,
    unlit = 4,
    lightsHeader = 5,
    depthOnly = 6,
    FBDisplayer = 7,
    projection = 8,
    skinned_projection = 9,
    vertexHeader = 10
};