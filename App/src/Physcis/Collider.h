#pragma once

typedef enum {
	Shape_Rect,
	Shape_Circle,
	Shape_Polygon,
} Shape;

typedef struct {
	vec2s min;
	vec2s max;
} AABB;

void AABB_Transform(AABB* aabb, vec2s position);
vec2s AABB_GetCenter(AABB aabb);
vec2s AABB_GetDimensions(AABB aabb);
void AABB_Set(AABB* aabb, vec2s center, vec2s size);

typedef struct Object Object;

typedef struct {

	//float size;
	float dynamicFriction;
	float staticFriction;
	Shape shape;
	float area;

	AABB aabb;

	union{
		vec2s* vertices;
		float radius;
	};
	int vertexCount;
	Object* object;




} Collider;

int AABB_Intersection(AABB quadrant, AABB point);

void Collider_Init_Circle(Object* object, float radius);
void Collider_Init_Poly(Object* object, vec2s* vertices, int vertexCount);
void Collider_Init_Rect(Object* object, float width,  float height);

float Collider_Get_Area(Collider* collider);

void ProjectVertices(vec2s position, vec2s* vertices, int vertexCount, vec2s axis, float* min, float* max);
int Collider_Bounding_Box_Check(Object* a, Object* b);
int Collider_SAT_Collision_Check(Object* a, Object* b, vec2s* normal, float* depth);

