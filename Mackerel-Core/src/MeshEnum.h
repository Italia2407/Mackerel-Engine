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
    survivor_GLTF = 6,
    Suzanne = 7,
    Suzanne_GLTF = 8,
    TestMesh = 9,
    three_bone = 10,
    three_bone_2 = 11,
    three_bone_embedded = 12,
    three_bone_no_skeleton = 13,
    xbot = 14,
    xbot_binary = 15
};