#pragma once

enum Shape {
	Shape_Circle,
	Shape_Rect,
	Shape_Polygon,
};
enum Layer {
	
};
typedef struct {
	vec2s min;
	vec2s max;
} AABB;

typedef struct Object Object;

typedef struct {

	//float size;
	float dynamicFriction;
	float staticFriction;
	enum Shape shape;
	float area;

	AABB aabb;

	union{
		vec2s* vertices;
		float radius;
	};
	int vertexCount;
	Object* object;




} Collider;



void Collider_Init_Circle(Object* object, float radius);
void Collider_Init_Poly(Object* object, vec2s* vertices, int vertexCount);
void Collider_Init_Rect(Object* object, float width,  float height);

float Collider_Get_Area(Collider* collider);

int Collider_Bounding_Box_Check(Object* a, Object* b);
