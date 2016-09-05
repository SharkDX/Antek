#pragma once
#include <iostream>

namespace Antek
{
	struct Block
	{
	public:
		uint8_t id;
		unsigned int sunlight : 4;
		unsigned int blocklight : 4;
	};
}