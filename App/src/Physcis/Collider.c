
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <memory.h>

#include "Object.h"




void AABB_Transform(AABB* aabb, vec2s position)
{
	aabb->min.x += position.x;
	aabb->min.y += position.y;

	aabb->max.x += position.x;
	aabb->max.y += position.y;
}

vec2s AABB_GetDimensions(AABB aabb)
{
	vec2s dims = { aabb.max.x - aabb.min.x, aabb.max.y - aabb.min.y };
	return dims;
}

vec2s AABB_GetCenter(AABB aabb)
{
	vec2s scale = AABB_GetDimensions(aabb);
	vec2s center = {
		.x = aabb.max.x - scale.x/2,
		.y = aabb.max.y - scale.y/2,
	};
	return center;
}

void AABB_Set(AABB* aabb, vec2s center, vec2s size)
{
	vec2s two = { .x = 2.0f, .y = 2.0f };
	aabb->min = glms_vec2_sub(center, glms_vec2_div(size, two));
	aabb->max = glms_vec2_add(center, glms_vec2_div(size, two));
}

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

int AABB_Intersection(AABB quadrant, AABB point)
{
	return (!(quadrant.max.x <= point.min.x || point.max.x <= quadrant.min.x ||
		quadrant.max.y <= point.min.y || point.max.y <= quadrant.min.y));
}

int Collider_Bounding_Box_Check(Object* a, Object* b)
{
	vec2s positionA = a->position;
	vec2s positionB = b->position;

	AABB AABB_A = a->collider.aabb;
	AABB AABB_B = b->collider.aabb;

	AABB_A.min = glms_vec2_add(AABB_A.min, positionA);
	AABB_A.max = glms_vec2_add(AABB_A.max, positionA);

	AABB_B.min = glms_vec2_add(AABB_B.min, positionB);
	AABB_B.max = glms_vec2_add(AABB_B.max, positionB);
	
	return AABB_Intersection(AABB_A, AABB_B);
}
int Collider_Circle_Circle_Check() {

}
int Collider_Polygon_Circle_SAT_Check() {
	//printf("circl");
}
int Collider_Polygon_Polygon_SAT_Check(Object* a, Object* b, vec2s* normal, float* depth) {
	Collider* colliderA = &a->collider;
	Collider* colliderB = &b->collider;

	int vertexCountA = colliderA->vertexCount;
	int vertexCountB = colliderB->vertexCount;

	normal->x = normal->y = 0.0f;
	*depth = FLT_MAX;

	Object* obj = a;
	int halfway = 0;
	for (int i = 0; i < vertexCountA + vertexCountB; i++) {
		
		if (i >= vertexCountA) {
			obj = b;
			halfway = 1;
		}
		int vertex = i - (halfway * vertexCountA);

		vec2s vertexA = obj->collider.vertices[vertex];
		vec2s vertexB = obj->collider.vertices[(vertex + 1) % obj->collider.vertexCount];


		vec2s edge = { vertexB.x - vertexA.x, vertexB.y - vertexA.y };
		vec2s axis = { -edge.y, edge.x };
		axis = glms_vec2_normalize(axis);

		float maxA, minA,
			maxB, minB;

		ProjectVertices(a->position, colliderA->vertices, vertexCountA, axis, &minA, &maxA);
		ProjectVertices(b->position, colliderB->vertices, vertexCountB, axis, &minB, &maxB);

		if (minA >= maxB || minB >= maxA)
		{
			return 0;
		}

		float axisDepth = min(maxB - minA, maxA - minB);

		if (axisDepth < *depth)
		{
			*depth = axisDepth;
			*normal = axis;
		}
	}

	vec2s dir = { b->position.x - a->position.x, b->position.y - a->position.y };

	if (glms_vec2_dot(dir, *normal) < 0.0f)
	{
		normal->x = -normal->x;
		normal->y = -normal->y;
	}


	return 1;
}

typedef int(*CollisionTestFunction)(Object* a, Object* b, vec2s* normal, float* depth);

static const CollisionTestFunction collisionTests[2][2] = { 
	{ Collider_Polygon_Polygon_SAT_Check, Collider_Polygon_Circle_SAT_Check }, 
	{ Collider_Polygon_Circle_SAT_Check,  Collider_Circle_Circle_Check      } 
};

int Collider_SAT_Collision_Check(Object* a, Object* b, vec2s* normal, float* depth) {
	
	return collisionTests[a->collider.shape % 2][b->collider.shape % 2](a, b, normal, depth);
}

void ProjectVertices(vec2s position, vec2s* vertices, int vertexCount, vec2s axis, float* min, float* max) {
	*min = FLT_MAX;
	*max = -FLT_MAX;

	for (int i = 0; i < vertexCount; i++) {
		vec2s vertex = vertices[i];
		vertex.x += position.x; vertex.y += position.y;

		float proj = glms_vec2_dot(vertex, axis);
		//printf("axis: {%f, %f}\n", axis.x, axis.y);

		if (proj < *min) { *min = proj; }
		if (proj > *max) { *max = proj; }
		//printf("min: %f, max: %f}\n", *min, *max);
	}
}

float Collider_Get_Area(Collider* collider)
{
	return 0.0f;
}

