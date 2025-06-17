#include <stdio.h>
#include <malloc.h>

#include "Common.h"
#include "Window.h"

#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
	printf("GLFW error: %s", description);
	ASSERT(0);
}

static void window_close_callback(GLFWwindow* handle)
{
	Window* window = glfwGetWindowUserPointer(handle);
	window->is_open = 0;
}

static void window_resize_callback(GLFWwindow* handle, int width, int height)
{
	Window* window = glfwGetWindowUserPointer(handle);
	window->width = width;
	window->height = height;
	if (window->resizeCallback)
		window->resizeCallback(window, width, height);
}
static void window_mouse_button_callback(GLFWwindow* handle, int button, int action, int mods)
{
	Window* window = glfwGetWindowUserPointer(handle); 
	if (window->mouseButtonCallback)
		window->mouseButtonCallback(window, button, action, mods);
}
static void window_mouse_move_callback(GLFWwindow* handle, double x, double y)
{
	Window* window = glfwGetWindowUserPointer(handle);
	if (window->mouseMoveCallback)
		window->mouseMoveCallback(window, (int)x, (int)y);
}
static void window_mouse_scroll_callback(GLFWwindow* handle, double xdelta, double ydelta)
{
	Window* window = glfwGetWindowUserPointer(handle);
	if (window->mouseScrollCallback)
		window->mouseScrollCallback(window, (int)xdelta, (int)ydelta);
}
static void window_key_callback(GLFWwindow* handle, int key, int scancode, int action, int mods)
{
	Window* window = glfwGetWindowUserPointer(handle);
	if (window->keyCallback)
		window->keyCallback(window, key, action, mods);
}

Window* window_create(const char* title, int width, int height)
{
	glfwSetErrorCallback(glfw_error_callback);
	ASSERT(glfwInit() && "failed to initialize glfw");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* handle = glfwCreateWindow(width, height, title, 0, 0);
	ASSERT(handle && "failed to create window");
	glfwMakeContextCurrent(handle);

	int gladResult = gladLoadGL();
	ASSERT(gladResult && "failed to initialize glad");

	Window* window = calloc(1, sizeof(Window));
	window->handle = handle;
	window->width = width;
	window->height = height;
	window->is_open = 1;

	glfwShowWindow(handle);

	glfwSwapInterval(1);
	glfwSetWindowUserPointer(handle, window);

	// Set callbacks
	glfwSetWindowCloseCallback(handle, window_close_callback);
	glfwSetWindowSizeCallback(handle, window_resize_callback);
	glfwSetKeyCallback(handle, window_key_callback);
	glfwSetMouseButtonCallback(handle, window_mouse_button_callback);
	glfwSetCursorPosCallback(handle, window_mouse_move_callback);
	glfwSetScrollCallback(handle, window_mouse_scroll_callback);	

	return window;
}

void window_destroy(Window* window)
{
	glfwDestroyWindow(window->handle);
	window->handle = 0;

	glfwTerminate();
}

void window_poll_events()
{
	glfwPollEvents();
}

void window_swap_buffers(Window* window)
{
	glfwSwapBuffers(window->handle);
}

int window_is_open(Window* window)
{
	return window->is_open;
}

void window_resize(Window* window, int width, int height)
{
	glfwSetWindowSize(window->handle, width, height);
}