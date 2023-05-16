#pragma once

#include <cinttypes>
#include <string>
#include <unordered_map>

#undef max
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

namespace MCK
{
	struct MeshNode
	{
		MeshNode* parent = nullptr;
		std::vector<MeshNode*> children{};
	};

	struct SkinnedMeshData
	{
		tinygltf::Model gltfModel;
		std::unordered_map<std::string, uint16_t> animationIndices = {};

		void PopulateAnimationIndices();
		void ConstructNodeGraph();
	};
}