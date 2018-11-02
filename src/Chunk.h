#pragma once
#include <iostream>
#include <fstream>
#include "Config.h"
#include "LightingManager.h"
#include "world_generation\WorldGenerator.h"
#include "Block.h"

namespace Antek
{
	class Chunk
	{
	public:
		Chunk(int chunk_x, int chunk_z, WorldGeneration::WorldGenerator* world_generator);
		~Chunk();
		void Init();
		Block*** GetBlocksForRenderer();

		uint32_t GetSizeX();
		uint32_t GetSizeY();
		uint32_t GetSizeZ();
		Antek::Block GetBlock(int i, int j, int k);
	private:
		Block*** _blocks;
		int _chunk_x;
		int _chunk_z;
		int _min_height;
		int _max_height;
		uint32_t _size_x;
		uint32_t _size_y;
		uint32_t _size_z;
		WorldGeneration::WorldGenerator* _world_generator;

		unsigned int Antek::Chunk::GetLightValue(int i, int j, int k);
	};
}