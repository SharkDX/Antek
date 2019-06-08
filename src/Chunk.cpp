#include "Chunk.h"

Antek::Chunk::Chunk(int chunk_x, int chunk_z, WorldGeneration::WorldGenerator* world_generator)
{
	this->_chunk_x = chunk_x;
	this->_chunk_z = chunk_z;
	_size_x = CHUNK_SIZE;
	_size_y = WORLD_HEIGHT;
	_size_z = CHUNK_SIZE;
	_world_generator = world_generator;

	int min_height = WORLD_HEIGHT;
	int max_height = 0;
	int** height_map = new int*[_size_x];
	for (size_t i = 0; i < _size_x; i++)
	{
		height_map[i] = new int[_size_z];
		for (size_t k = 0; k < _size_z; k++)
		{
			//height_map[i][k] = _world_generator->GetHeight(_chunk_x + i, _chunk_z + k);
			height_map[i][k] = _size_y;
			if (height_map[i][k] > max_height)
				max_height = height_map[i][k];
			if (height_map[i][k] < min_height)
				min_height = height_map[i][k];
		}
	}
	_min_height = min_height;
	_max_height = max_height;

	_blocks = new Block**[_size_x];
	for(size_t i = 0; i < _size_x; i++)
	{
		_blocks[i] = new Block*[_size_z];
		for (size_t k = 0; k < _size_z; k++)
		{
			_blocks[i][k] = new Block[max_height - min_height + 1];
			for (size_t j = 0; j <= max_height - min_height; j++)
				_blocks[i][k][j].id = 0;
			for (size_t j = 0; j <= height_map[i][k] - min_height; j++)
				_blocks[i][k][j].id = 1;
				//_blocks[i][k][j].id = _world_generator->GetBiome(_chunk_x + i, _chunk_z + k)->GetBlockId(_chunk_x + i, min_height + j, _chunk_z + k, height_map[i][k]);
		}
	}
}

Antek::Chunk::~Chunk()
{
	for(size_t i = 0; i < _size_x; i++)
	{
		for (size_t k = 0; k < _size_z; k++)
		{
			delete _blocks[i][k];
		}
		delete [] _blocks[i];
	}
	delete [] _blocks;
}

void Antek::Chunk::Init()
{
}

unsigned int Antek::Chunk::GetLightValue(int i, int j, int k)
{
	return _blocks[i][j][k].sunlight;
}

Antek::Block*** Antek::Chunk::GetBlocksForRenderer()
{
	return _blocks;
}

uint32_t Antek::Chunk::GetSizeX()
{
	return _size_x;
}

uint32_t Antek::Chunk::GetSizeY()
{
	return _size_y;
}

uint32_t Antek::Chunk::GetSizeZ()
{
	return _size_z;
}

Antek::Block Antek::Chunk::GetBlock(int i, int j, int k)
{
	if (j < _min_height || j > _max_height) {
		Block b = Block();
		b.id = 0;
		return b;
	}
	return _blocks[i % CHUNK_SIZE][k % CHUNK_SIZE][j - _min_height];
}
