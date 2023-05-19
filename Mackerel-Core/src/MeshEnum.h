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
    Suzanne = 5,
    Suzanne_GLTF = 6,
    TestMesh = 7,
    three_bone = 8,
    three_bone_2 = 9,
    three_bone_embedded = 10,
    three_bone_no_skeleton = 11,
    xbot = 12
};