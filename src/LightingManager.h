#pragma once
#include "Block.h"

namespace Antek
{
	class LightingManager
	{
	public:
		LightingManager()
		{

		}

		LightingManager(const uint32_t x_size, const uint32_t y_size, const uint32_t z_size)
		{
			_size_x = x_size;
			_size_y = y_size;
			_size_z = z_size;
		}

		void InitLightning(Block*** blocks)
		{
			for (size_t i = 0; i < _size_x; i++)
			{
				for (size_t j = 0; j < _size_y; j++)
				{
					for (size_t k = 0; k < _size_z; k++)
					{
						if (blocks[i][k][j].id)
						{
							blocks[i][k][j].blocklight = 0;
							blocks[i][k][j].sunlight = 255;
						}
					}
				}
			}
			for (size_t i = 0; i < _size_x; i++)
			{
				for (size_t k = 0; k < _size_z; k++)
				{
					int top = 0;
					for (size_t j = _size_y - 1; j > 0; j--)
					{
						if (blocks[i][k][j].id)
						{
							top = j;
							break;
						}
					}
					blocks[i][k][top + 1].id = 0;
					blocks[i][k][top + 1].sunlight = 255;
				}
			}
		}


	private:
		uint32_t _size_x;
		uint32_t _size_y;
		uint32_t _size_z;
	};
}