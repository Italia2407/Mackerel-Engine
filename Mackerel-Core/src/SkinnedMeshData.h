#pragma once

#include <cinttypes>
#include <string>
#include <map>

#define BULLET_IMPLEMENTED
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>

namespace MCK
{
	struct SkinnedMeshData
	{
		std::map<std::string, ozz::animation::Animation> animations = {};
		uint32_t max_channels = 0;

		ozz::animation::Skeleton skeleton;
	};
}