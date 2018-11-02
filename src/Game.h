#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "graphics\renderers\BlockRenderer.h"
#include "utils\SystemResources.h"
#include "utils\Camera.h"
#include "World.h"
#include "BlockFactory.h"
#include "utils\Profiler.h"

namespace Antek
{
	class Game
	{
	public:
		Game(GLFWwindow* window);
		~Game();

		void LoadResources();
		void Initialize();
		void Run();

		// callbacks from glfw
		void ScrollWheelCallback(double x, double y);
		void KeyCallback(int key, int scancode, int action, int mods);
		void MouseButtonCallback(int button, int action, int bits);
		void CharCallback(unsigned int keycode);

	private:
		bool _is_running;
		int _screen_width;
		int _screen_height;
		GLFWwindow* _window; // Game instance is not responsible for cleaning this
		Utils::Camera* _camera;
		World* _world;
		Renderers::BlockRenderer* _block_renderer;
		Utils::Profiler* _profiler;

		void Update(float deltaTime);
		void Render();
	};
}