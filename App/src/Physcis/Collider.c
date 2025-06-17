
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <memory.h>
#include "Object.h"


void Collider_Init_Circle(Object* object, float radius)
{
	object->collider.area = GLM_PI * radius * radius;
}

void Collider_Init_Poly(Object* object, vec2s* vertices, int vertexCount)
{

}

void Collider_Init_Rect(Object* object, float width, float height)
{

	Collider* collider = &object->collider;

	collider->dynamicFriction = 0.67f;
	collider->staticFriction = 0.67f;

	collider->shape = Shape_Rect;
	collider->vertices = malloc(4*sizeof(vec2s));
	collider->vertexCount = 4;

	collider->vertices[0].x = -width / 2;
	collider->vertices[0].y = height / 2;

	collider->vertices[1].x = width / 2;
	collider->vertices[1].y = height / 2;

	collider->vertices[2].x = -width / 2;
	collider->vertices[2].y = -height / 2;

	collider->vertices[3].x = width / 2;
	collider->vertices[3].y = height / 2;

	collider->area = width * height;

	object->mass = object->density * collider->area;

	Object_Calculate_Inertia(collider->object);
}