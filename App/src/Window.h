#pragma once

#include "Events.h"

struct Window
{
	void* handle; // GLFWwindow*
	int width, height;

	// Set these to a function to have it called upon the corresponding event
	WindowResizeCallback resizeCallback;
	WindowCloseCallback closeCallback;
	MouseButtonCallback mouseButtonCallback;
	MouseMoveCallback mouseMoveCallback;
	MouseScrollCallback mouseScrollCallback;
	KeyCallback keyCallback;

	int is_open;
};

Window* window_create(const char* title, int width, int height);
void window_destroy(Window* window);

void window_poll_events();
void window_swap_buffers(Window* window);

int window_is_open(Window* window);

void window_resize(Window* window, int width, int height);