#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include "graphics\renderers\BlockRenderer.h"
#include "utils\SystemResources.h"
#include "utils\Camera.h"
#include "Block.h"
#include "World.h"

namespace Antek
{
	struct BlockType
	{
	public:
		uint8_t brightness;
		uint8_t r, g, b;
	};

	class BlockFactory
	{
	public:
		static void InitBlockTypes();
		static glm::vec3 GetBlockColor(uint8_t id);

	private:
		static BlockType* _block_types[];

		// Disallow creating an instance
		BlockFactory() {};
	};
}