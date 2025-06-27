
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <memory.h>

#include "Object.h"


void Collider_Init_Circle(Object* object, float radius)
{
	Collider* collider = &object->collider;

	collider->dynamicFriction = 0.67f;
	collider->staticFriction = 0.67f;
	collider->shape = Shape_Circle;
	collider->radius = radius;

	collider->aabb.min.x = -radius;
	collider->aabb.min.y = -radius;
	collider->aabb.max.x = radius;
	collider->aabb.max.y = radius;

	object->collider.area = GLM_PI * radius * radius;
	object->mass = object->density * collider->area;

	Object_Calculate_Inertia(collider->object);
}

void Collider_Init_Poly(Object* object, vec2s* vertices, int vertexCount)
{
	Collider* collider = &object->collider;

	collider->dynamicFriction = 0.67f;
	collider->staticFriction = 0.67f;
	collider->vertexCount = vertexCount;
	collider->shape = Shape_Polygon;

	collider->vertices = malloc(vertexCount * sizeof(vec2s));
	memcpy(collider->vertices, vertices, vertexCount * sizeof(vec2s));
	
	//todo: CALCULATE AABB

	//get area
	object->mass = object->density * collider->area;

	Object_Calculate_Inertia(collider->object);
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

	collider->aabb.min.x = -width / 2;
	collider->aabb.min.y = -height / 2;
	collider->aabb.max.x = width / 2;
	collider->aabb.max.y = height / 2;

	object->mass = object->density * collider->area;

	Object_Calculate_Inertia(collider->object);
}

int Collider_Bounding_Box_Check(Object* a, Object* b)
{
	vec2s positionA = a->position;
	vec2s positionB = b->position;

	AABB AABB_A = a->collider.aabb;
	AABB AABB_B = b->collider.aabb;
	//AABB aaabb = entityA.GetCollider()->GetAABB();

	//aabb pos
	vec2s Min_A = { AABB_A.min.x + positionA.x, AABB_A.min.y + positionA.y };
	vec2s Max_A = { AABB_A.max.x + positionA.x, AABB_A.max.y + positionA.y };

	vec2s Min_B = { AABB_B.min.x + positionB.x, AABB_B.min.y + positionB.y };
	vec2s Max_B = { AABB_B.max.x + positionB.x, AABB_B.max.y + positionB.y };

	if (Max_A.x <= Min_B.x || Max_B.x <= Min_A.x ||
		Max_A.y <= Min_B.y || Max_B.y <= Min_A.y)
		return 0;
	return 1;
}

float Collider_Get_Area(Collider* collider)
{
	return 0.0f;
}

