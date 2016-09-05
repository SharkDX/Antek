#pragma once
#include "windows.h"
#include "psapi.h"

namespace Antek
{
	namespace Utils
	{
		class SystemResources
		{
		public:
			static int get_total_memory_usage()
			{
				PROCESS_MEMORY_COUNTERS pmc;
				GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
				return pmc.WorkingSetSize;
			};
		};
	}
}