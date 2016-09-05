#pragma once
#define WORLD_SIZE 20
#include <iostream>
#include <fstream>
#include "world_generation\WorldGenerator.h"
#include "Block.h"

namespace Antek
{
	class World
	{
	public:
		World(const uint32_t x_size, const uint32_t y_size, const uint32_t z_size);
		~World();
		void Init();
		Block**** GetBlocksForRenderer();

		uint32_t GetSizeX();
		uint32_t GetSizeY();
		uint32_t GetSizeZ();
	private:
		Block**** _blocks;
		uint32_t _size_x;
		uint32_t _size_y;
		uint32_t _size_z;
		WorldGeneration::WorldGenerator* _world_generator;
	};
}