#pragma once
#include <glm/glm.hpp>
#include <GLFW\glfw3.h>
#include <string>
#include <iostream>
#include <unordered_map>

namespace Antek
{
	namespace Utils
	{
		class Profiler
		{
		public:
			Profiler()
			{
			};

			Profiler(const Profiler&) = delete;
			Profiler& operator=(const Profiler&) = delete;

			void StartNewProfile(const std::string& name)
			{
				profiles.insert(std::make_pair<std::string, double>(name.c_str(), glfwGetTime()));
			};

			void EndProfile(const std::string& name)
			{
				auto it = profiles.find(name.c_str());

				assert (it != profiles.end());

				double diff_time = glfwGetTime() - it->second;
				printf("%s took %g seconds.\n", name.c_str(), diff_time);
			};

		private:
			std::unordered_map<std::string, double> profiles;

		};
	}
}