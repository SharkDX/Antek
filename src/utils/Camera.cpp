#include "Camera.h"

Antek::Utils::Camera::Camera(GLFWwindow* window)
{
	this->_window = window;
	glfwGetWindowSize(window, &_screen_width, &_screen_height);
    init();
}

Antek::Utils::Camera::Camera(const glm::vec3 &position)
{
    init();
    setPosition(position);
}

void Antek::Utils::Camera::init()
{
    this->Projection = glm::perspective(60.0f, _screen_width / (float)_screen_height, 0.1f, 2000.0f);
    this->position = glm::vec3(0.0f, 128.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    horizontalAngle = 0;
    verticalAngle = 0;
}

void Antek::Utils::Camera::Update(float elapsed)
{
	double xpos, ypos;
	glfwGetCursorPos(_window, &xpos, &ypos);
	
	int wX, wY;
    glfwGetWindowSize(_window, &wX, &wY);

	glfwSetCursorPos(_window, wX / 2, wY / 2);

	horizontalAngle += (float(wX / 2) - xpos) * 0.002f; 
	verticalAngle += (float(wY / 2) - ypos) * 0.002f;

	if (verticalAngle > 3.14f / 2.0f)
		verticalAngle = 3.14f / 2.0f;
	if (verticalAngle < -3.14f / 2.0f)
		verticalAngle = -3.14f / 2.0f;

    float speed = 0.1f;
    if(glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed *= 5;
	if(glfwGetKey(_window, 'W') == GLFW_PRESS)
        moveAhead(speed);
	if(glfwGetKey(_window, 'A') == GLFW_PRESS)
		moveLeft(speed);
	if(glfwGetKey(_window, 'D') == GLFW_PRESS)
		moveRight(speed);
	if(glfwGetKey(_window, 'S') == GLFW_PRESS)
		moveBack(speed);

	calculateVectors();
	this->View = getView();
}

glm::mat4 Antek::Utils::Camera::getView()
{
    return glm::lookAt(position, position + direction, up);
}

glm::mat4 Antek::Utils::Camera::getMVP()
{
	return this->Projection * this->View;
}

void Antek::Utils::Camera::moveAhead(float speed)
{
    position += direction * speed;
}

void Antek::Utils::Camera::moveBack(float speed)
{
    position -= direction * speed;
}

void Antek::Utils::Camera::moveLeft(float speed)
{
    position += glm::cross(up, direction) * speed;
}

void Antek::Utils::Camera::moveRight(float speed)
{
    position -= glm::cross(up, direction) * speed;
}

void Antek::Utils::Camera::calculateVectors()
{
    direction.x = glm::cos(verticalAngle) * glm::sin(horizontalAngle);
    direction.y = glm::sin(verticalAngle);
    direction.z = glm::cos(verticalAngle) * glm::cos(horizontalAngle);
	
    right = glm::vec3(glm::sin(horizontalAngle - 3.14f/2.0f), 0, glm::cos(horizontalAngle - 3.14f/2.0f));
    up = glm::cross(right, direction);
}

glm::vec3 Antek::Utils::Camera::getPosition()
{
    return position;
}

void Antek::Utils::Camera::setPosition(const glm::vec3 &position)
{
    this->position = position;
}

float Antek::Utils::Camera::getHorizontalAngle()
{
    return horizontalAngle;
}

float Antek::Utils::Camera::getVerticalAngle()
{
    return verticalAngle;    
}
