#pragma once

#include <cinttypes>
#include <string>
#include <map>

#undef max
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

#define BULLET_IMPLEMENTED
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>

namespace MCK
{
	struct SkinnedMeshData
	{
		tinygltf::Model gltfModel;

		std::map<std::string, ozz::animation::Animation> animations = {};
		uint32_t max_channels = 0;

		ozz::animation::Skeleton skeleton;
	};
}