#pragma once
#include <iostream>
#include <assert.h>
#include "open-simplex-noise.h"
#include "..\Block.h"

namespace Antek
{
	namespace WorldGeneration
	{
		class WorldGenerator
		{
		public:
			WorldGenerator(const uint32_t x_size, const uint32_t y_size, const uint32_t z_size, const int seed);
			virtual ~WorldGenerator();
			WorldGenerator(const WorldGenerator&) = delete;
			WorldGenerator& operator=(const WorldGenerator&) = delete;

			void GenerateWorld(Block**** blocks);
		private:
			float normal_float(float f);

			int _seed;
			uint32_t _size_x;
			uint32_t _size_y;
			uint32_t _size_z;

			struct osn_context* _ctx;
		};
	}
}