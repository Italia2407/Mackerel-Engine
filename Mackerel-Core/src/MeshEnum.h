#pragma once
enum class MeshEnum
{
    __MCK_DISPLAY_SCREEN = -2,
    __MCK_DEFAULT = -1,
    // Negative values are reserved for engine use
    DisplayScreen = 0,
    Icosphere = 1,
    MackerelText = 2,
    MackerelText_embedded = 3,
    cube = 4,
    survivor = 5,
    Suzanne = 6,
    Suzanne_GLTF = 7,
    TestMesh = 8,
    three_bone = 9,
    three_bone_2 = 10,
    three_bone_embedded = 11,
    three_bone_no_skeleton = 12,
    xbot = 13,
    xbot_binary = 14
};