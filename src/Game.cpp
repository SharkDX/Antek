#include "Game.h"
#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Antek::Game::Game(GLFWwindow* window)
{
	glfwGetWindowSize(window, &_screen_width, &_screen_height);

	_is_running = false;
	_window = window;
}

Antek::Game::~Game()
{
	
}

void Antek::Game::LoadResources()
{
}

void Antek::Game::Initialize()
{
	// OpenGL setup
	glClearColor(0.1f, 0.4f, 0.8f, 1);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// Input setup
	// TODO make a custom cursor
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Init basic mechanics
	BlockFactory::InitBlockTypes();

	_camera = new Utils::Camera(_window);
	_world = new World(350, 128, 350);
	_block_renderer = new Renderers::BlockRenderer();
	_block_renderer->Init(_world->GetBlocksForRenderer(), _world->GetSizeX(), _world->GetSizeY(), _world->GetSizeZ());
}

void Antek::Game::Update(float deltaTime)
{
	_camera->Update(deltaTime);
}

void Antek::Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4x4 mvp = _camera->getMVP();

	_block_renderer->Render(mvp);
}

void Antek::Game::Run()
{
	int frames = 0;
	double lastFrame = glfwGetTime();
	double deltaTime = 0;
	double lastUpdate = glfwGetTime();

	while (!glfwWindowShouldClose(_window) && glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		frames++;
		deltaTime = glfwGetTime() - lastUpdate;
		lastUpdate = glfwGetTime();

		if (glfwGetTime() - lastFrame >= 1.0)
		{
			printf("FPS: %d, Ram: %d MB\n", frames, Utils::SystemResources::get_total_memory_usage() / 1000 / 1000);
			lastFrame = glfwGetTime();
			frames = 0;
		}

		Update((float)deltaTime);
		Render();

		glfwSwapBuffers(_window);
		glfwPollEvents();

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void Antek::Game::ScrollWheelCallback(double x, double y)
{
}

void Antek::Game::KeyCallback(int key, int scancode, int action, int mods)
{
}

void Antek::Game::MouseButtonCallback(int button, int action, int bits)
{
}

void Antek::Game::CharCallback(unsigned int keycode)
{
}