#include "BlockFactory.h"

Antek::BlockType* Antek::BlockFactory::_block_types[UINT8_MAX];

void Antek::BlockFactory::InitBlockTypes()
{
	for (int i = 0; i < UINT8_MAX; i++)
	{
		_block_types[i] = nullptr;
	}

	_block_types[1] = new BlockType();
	_block_types[1]->brightness = 0;
	_block_types[1]->r = 128;
	_block_types[1]->g = 128;
	_block_types[1]->b = 128;
}

glm::vec3 Antek::BlockFactory::GetBlockColor(uint8_t id)
{
	return glm::vec3(_block_types[id]->r / 255.0f, _block_types[id]->g / 255.0f, _block_types[id]->b / 255.0f);
}