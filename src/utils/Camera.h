#pragma once
#include <glm/glm.hpp>
#include <GLFW\glfw3.h>
#include <glm/ext.hpp>

namespace Antek
{
	namespace Utils
	{
		class Camera
		{
		public:
			glm::mat4 Projection;
			glm::mat4 View;

			Camera(GLFWwindow* window);
			Camera(const glm::vec3 &position);
			glm::mat4 getView();
			glm::mat4 getMVP();
			void moveAhead(float speed);
			void moveBack(float speed);
			void moveLeft(float speed);
			void moveRight(float speed);
			void calculateVectors();
			void Update(float elapsed);
			glm::vec3 getPosition();
			void setPosition(const glm::vec3 &position);
			float getHorizontalAngle();
			float getVerticalAngle();

		private:
			void init();

			float horizontalAngle;
			float verticalAngle;

			int _screen_width;
			int _screen_height;

			glm::vec3 position;
			glm::vec3 direction;
			glm::vec3 up;
			glm::vec3 right;
			GLFWwindow* _window;
		};
	}
}