#pragma once
enum class MeshEnum
{
    __MCK_DISPLAY_SCREEN = -2,
    __MCK_DEFAULT = -1,
    // Negative values are reserved for engine use
    DisplayScreen = 0,
    MackerelText = 1,
    MackerelText_embedded = 2,
    cube = 3,
    Suzanne = 4,
    TestMesh = 5,
    three_bone = 6,
    three_bone_2 = 7,
    three_bone_embedded = 8,
    three_bone_no_skeleton = 9
};