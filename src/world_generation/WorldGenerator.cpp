#include "WorldGenerator.h"

Antek::WorldGeneration::WorldGenerator::WorldGenerator(const uint32_t x_size, const uint32_t y_size, const uint32_t z_size, const int seed)
{
	_size_x = x_size;
	_size_y = y_size;
	_size_z = z_size;
	this->_seed = seed;

	open_simplex_noise(_seed, &_ctx);
}

Antek::WorldGeneration::WorldGenerator::~WorldGenerator()
{
	open_simplex_noise_free(_ctx);
}

void Antek::WorldGeneration::WorldGenerator::GenerateWorld(Block**** blocks)
{
	for (int i = 0; i < _size_x; i++)
	{
		for (int k = 0; k < _size_z; k++)
		{
			/*float noise = open_simplex_noise2(_ctx, i / 40.0f, k / 40.0f);
			float octave = normal_float(open_simplex_noise2(_ctx, 12000 + i / 200.0f, 12000 + k / 200.0f));
			noise *= pow(octave + 0.5f, 2) / pow(1.5f, 2);
			noise *= pow(octave + 0.5f, 2) / pow(1.5f, 2);
			noise = normal_float(noise);*/
			float noise = 1.0f;
			for (int j = 0; j < (int)(_size_y * noise); j++)
			{
				float n = (32 - j) / 64.0f + open_simplex_noise3(_ctx, i / 40.0f, j / 40.0f, k / 40.0f) / 1.0f;
				if (n > 0.0f)
				{
					blocks[i][k][j] = new Block();
					blocks[i][k][j]->id = 1;
				}
			}
		}
	}
}

float Antek::WorldGeneration::WorldGenerator::normal_float(float f)
{
	return f / 2.0f + 0.5f;
}
