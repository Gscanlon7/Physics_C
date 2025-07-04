#include <stdio.h>
#include <stdint.h>

#include "Window.h"
#include "Graphics.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <time.h>

#include "Physcis/PhysicsWorld.h"

static void window_resize_callback(Window* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

Window* window;
static float get_aspect_ratio()
{
	return (float)window->width / (float)window->height;
}

static float random_float(float max)
{
	return (float)rand() / (float)(RAND_MAX / max);
}

static vec3s random_color()
{
	vec3s result = { .x = random_float(1.0f), .y = random_float(1.0f), .z = random_float(1.0f) };
	return result;
}
//void draw_quadrants(Quadrant quad);
void draw_quadrants(Quadrant quad) {

	vec3s rectangleColor = { 0.2f, 1.0f, 1.0f };

	AABB aabb = quad.aabb;
	vec2s center = AABB_GetCenter(aabb);
	vec2s dim = AABB_GetDimensions(aabb);
	graphics_draw_square(center.x, center.y, dim.x, dim.y, 0.0f, rectangleColor);


	if (quad.lastCell)
		return;

	for (int i = 0; i < 4; i++) {
		draw_quadrants(quad.subQuadrants[i]);
	}
}
int main()
{	
	srand(time(NULL));

	window = window_create("Window", 1870, 1050);
	window->resizeCallback = window_resize_callback;
	
	//window->mouseScrollCallback = mouse_scroll_callback;
	graphics_init();
	float cameraZoom = 10;

	World_Init();

	float fps = 144.0f;
	float deltaTime = 1.0f / fps;
	float accumulator = 0.0f;

	float frameStart = glfwGetTime();

	vec3s random_colors[4];
	random_colors[0] = random_color();
	random_colors[1] = random_color();
	random_colors[2] = random_color();

	Subdivide_Quadrant(&world, &world.rootQuadrant);
	Subdivide_Quadrant(&world, &world.rootQuadrant.subQuadrants[0]);
	Subdivide_Quadrant(&world, &world.rootQuadrant.subQuadrants[0].subQuadrants[1]);

	while (window_is_open(window))
	{
		
		float currentTime = glfwGetTime();
		accumulator += currentTime - frameStart;

		frameStart = currentTime;

		if (accumulator > 0.2f)
			accumulator = 0.2f;

		while (accumulator > deltaTime) {
			World_Step(deltaTime);
			accumulator -= deltaTime;
		}

		window_poll_events();

		glClearColor(0.08f, 0.07f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		graphics_update_camera(0.0f, 0.0f, cameraZoom, get_aspect_ratio());
		
		//printf("%f\n", deltaTime);

		// Render sht
		{
			draw_quadrants(world.rootQuadrant);
				
			for (int i = 0; i < world.objectCount; i++) {
				Object* obj = &world.objectList[i];
				
				switch (obj->collider.shape) {
					case Shape_Rect:
					{
						float width = obj->collider.vertices[1].x - obj->collider.vertices[0].x;
						float height = obj->collider.vertices[1].y - obj->collider.vertices[2].y;
						graphics_draw_square(obj->position.x, obj->position.y, width, height, obj->rotation, random_colors[i]);
						break;
					}
					case Shape_Circle: 
					{
						graphics_draw_circle(obj->position.x, obj->position.y, obj->collider.radius, obj->rotation, random_colors[i]);
						break;
					}
					case Shape_Polygon:
					{

						break;
					}
				}
				
			}
			//graphics_draw_square(6.0f, 0.0f, 1.0f, 1.0f, 0.0f, color);
			//graphics_draw_square(6.0f, 10.0f, 1.0f, 1.0f, 0.0f, color);
		}

		window_swap_buffers(window);

	}
	graphics_shutdown();
	window_destroy(window);
}
