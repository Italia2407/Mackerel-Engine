#pragma once

#include <cinttypes>
#include <string>
#include <map>

#define BULLET_IMPLEMENTED
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/maths/soa_float4x4.h>

namespace MCK
{
	struct SkinnedMeshData
	{
		std::map<std::string, ozz::animation::Animation> animations = {};
		uint32_t max_channels = 0;

		ozz::animation::Skeleton skeleton;

		std::vector<ozz::math::Float4x4> inverseBindMatrices = {};

	};
}