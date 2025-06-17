#include "PhysicsWorld.h"

struct PhysicsWorld world;

void World_Init() {
	world.objectCount = 0;
	world.objectList = calloc(64, sizeof(Object));

	//create objects
	Object* obj = &world.objectList[world.objectCount++];
	Object_Init(obj);
	//adj obj prop
	Collider_Init_Rect(obj, 1.0f, 1.0f);



	



}

void World_Step(float deltaTime)
{
	//for (int currentIteration = 0; currentIteration < totalIteration; currentIteration++) {
		//reset
	//}


		Step_Bodies(deltaTime);
		Narrow_Phase();
		Broad_Phase();
}

void Step_Bodies(float deltaTime)
{
	for (int i = 0; i < world.objectCount; i++) {
		Object_Step(&world.objectList[i], deltaTime, -9.81f);
	}
}

void Narrow_Phase()
{
}

void Broad_Phase()
{
}
