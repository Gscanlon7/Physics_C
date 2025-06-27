#include "PhysicsWorld.h"

struct PhysicsWorld world;

void World_Init() {
	world.objectCount = 0;
	world.objectList = calloc(64, sizeof(Object));

	//create objects
	Object* ground = &world.objectList[world.objectCount++];
	Object_Init(ground);
	ground->position.y = -10.0f;
	ground->isStatic = 1;
	//adj obj prop
	Collider_Init_Rect(ground, 100.0f, 1.0f);

	Object* objRect = &world.objectList[world.objectCount++];
	Object_Init(objRect);
	//adj obj prop
	Collider_Init_Rect(objRect, 1.0f, 1.0f);


	Object* objCirc = &world.objectList[world.objectCount++];
	Object_Init(objCirc);
	objCirc->position.x = 5.0f;
	Collider_Init_Circle(objCirc, 0.5f);
	


	



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
	for (int i = 0; i < world.objectCount - 1; i++) {
		Object* a = &world.objectList[i];

		for (int j = i + 1; j < world.objectCount; j++) {
			Object* b = &world.objectList[j];

			if (Collider_Bounding_Box_Check(a, b)) {
				printf("obj");
			}
		}
	}
}
 