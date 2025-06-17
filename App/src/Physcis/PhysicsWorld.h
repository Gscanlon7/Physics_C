#pragma 
#include "Object.h"
struct PhysicsWorld {
	float gravity;
	float deltaTime;

	int objectCount;
	Object* objectList;
};

extern struct PhysicsWorld world;

void World_Init();
void World_Step(float deltaTime);
void Step_Bodies(float deltaTime);
void Narrow_Phase();
void Broad_Phase();