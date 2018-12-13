#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "shader.h"
#include "Transform.hpp"
#include "City.h"
#include "Geometry.hpp"
#include "Camera.h"
#include "Building.h"

class Transform;

class Window
{
public:
	static int width;
	static int height;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void DoMovement();

	static bool leftClick;
	static double curxPos;
	static double curyPos;

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	static glm::vec3 trackBallMapping(glm::vec2);
};

#endif
