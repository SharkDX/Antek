#include "World.h"

Antek::World::World(const uint32_t x_size, const uint32_t y_size, const uint32_t z_size)
{
	_size_x = x_size;
	_size_y = y_size;
	_size_z = z_size;

	int r = rand();
	_world_generator = new WorldGeneration::WorldGenerator(_size_x, _size_y, _size_z, r);

	_blocks = new Block***[_size_x];
	for(int i = 0; i < _size_x; i++)
	{
		_blocks[i] = new Block**[_size_z];
		for (int k = 0; k < _size_z; k++)
		{
			_blocks[i][k] = new Block*[_size_y];
			for (int j = 0; j < _size_y; j++)
				_blocks[i][k][j] = nullptr; // TODO make it more elegant.
		}
	}

	_world_generator->GenerateWorld(_blocks);
}

Antek::World::~World()
{
	for(int i = 0; i < _size_x; i++)
	{
		for (int k = 0; k < _size_z; k++)
		{
			delete _blocks[i][k];
		}
		delete [] _blocks[i];
	}
	delete [] _blocks;
}

void Antek::World::Init()
{
}

void Antek::World::CalculateLights()
{

}

Antek::Block**** Antek::World::GetBlocksForRenderer()
{
	return _blocks;
}

uint32_t Antek::World::GetSizeX()
{
	return _size_x;
}

uint32_t Antek::World::GetSizeY()
{
	return _size_y;
}

uint32_t Antek::World::GetSizeZ()
{
	return _size_z;
}