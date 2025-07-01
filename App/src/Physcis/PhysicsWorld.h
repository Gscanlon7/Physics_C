#pragma 

#include "Object.h"

typedef struct { Object* a; Object* b; } ContactPair;

typedef struct Quadrant Quadrant;

struct Quadrant
{
	AABB aabb;

	Object* objects[8];
	int objectCount;
	int lastCell;
	struct Quadrant* subQuadrants;
};

struct PhysicsWorld {
	int width, height;

	float gravity;
	float deltaTime;

	int objectCount;
	Object* objectList;

	Quadrant rootQuadrant;

	int contactPairCount;
	ContactPair* contactPairList;
};
extern struct PhysicsWorld world;

void World_Init();
void Subdivide_Quadrant(struct PhysicsWorld* world, Quadrant* main);
void World_Step(float deltaTime);
void Step_Bodies(float deltaTime);
void Broad_Phase();
void Narrow_Phase();
void SeperateBodies(Object* a, Object* b, vec2s mtv);