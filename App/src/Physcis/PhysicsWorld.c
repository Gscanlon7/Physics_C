#include "PhysicsWorld.h"

struct PhysicsWorld world;


void World_Init() {
	world.height = 10;
	world.width = 30;


	world.objectCount = 0;
	world.objectList = calloc(64, sizeof(Object));

	world.contactPairCount = 0;
	world.contactPairList = calloc(64, sizeof(ContactPair));

	AABB aabb = { {0.0f,0.0f}, {0.0f,0.0f} };

	vec2s pos = {0.0f, 0.0f};
	vec2s size = {world.width, world.height};
	AABB_Set(&aabb, pos, size);
	world.rootQuadrant.aabb = aabb;

	//create objects
	Object* ground = &world.objectList[world.objectCount++];
	Object_Init(ground);
	ground->position.y = -10.0f;
	ground->isStatic = 1;
	//ground->rotation = 1.0f;
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

void Subdivide_Quadrant(struct PhysicsWorld* world, Quadrant* main)
{
	main->subQuadrants = malloc(sizeof(Quadrant) * 4);
	main->lastCell = 0;
	vec2s currentDims = AABB_GetDimensions(main->aabb);
	vec2s currentCenter = AABB_GetCenter(main->aabb);

	vec2s two = { 2.0f, 2.0f };
	vec2s four = { 4.0f, 4.0f };
	vec2s newDims = glms_vec2_div(currentDims, two);
	{
		AABB* quad0 = &main->subQuadrants[0].aabb;
		
		vec2s newCenter = currentCenter;
		newCenter.x -= newDims.x/2;
		newCenter.y += newDims.y/2;


		AABB_Set(quad0, newCenter, newDims);
	}

	{
		AABB* quad1 = &main->subQuadrants[1].aabb;

		vec2s newCenter = currentCenter;
		newCenter.x += newDims.x/2;
		newCenter.y += newDims.y/2;


		AABB_Set(quad1, newCenter, newDims);
	}

	{
		AABB* quad2 = &main->subQuadrants[2].aabb;

		vec2s newCenter = currentCenter;
		newCenter.x -= newDims.x/2;
		newCenter.y -= newDims.y/2;


		AABB_Set(quad2, newCenter, newDims);
	}
	{
		AABB* quad3 = &main->subQuadrants[3].aabb;

		vec2s newCenter = currentCenter;
		newCenter.x += newDims.x/2;
		newCenter.y -= newDims.y/2;


		AABB_Set(quad3, newCenter, newDims);
	}


	 

	for (int q = 0; q < 4; q++)
	{
		Quadrant* sub = &main->subQuadrants[q];

		for (int i = 0; i < main->objectCount; i++)
		{
			Object* object = main->objects[i];
			AABB transformed = object->collider.aabb;
			AABB_Transform(&transformed, object->position);
			
			if (!AABB_Intersection(transformed, sub->aabb))
			{
				continue;
			}
			 
			if (!sub) __debugbreak(); // crash
			
			// TODO: multiple containing quadrants
			sub->objects[sub->objectCount++] = object;
		}
	}
}

void World_Step(float deltaTime)
{
	//for (int currentIteration = 0; currentIteration < totalIteration; currentIteration++) {
		//reset
	//}
	
		world.contactPairCount = 0;
		world.contactPairList = calloc(64, sizeof(ContactPair));

		Step_Bodies(deltaTime);
		Broad_Phase();
		Narrow_Phase();
}

void Step_Bodies(float deltaTime)
{
	for (int i = 0; i < world.objectCount; i++) {
		Object_Step(&world.objectList[i], deltaTime, -9.81f);
	}
}


void Broad_Phase()
{
	for (int i = 0; i < world.objectCount - 1; i++) {
		Object* a = &world.objectList[i];

		for (int j = i + 1; j < world.objectCount; j++) {
			Object* b = &world.objectList[j];

			if (Collider_Bounding_Box_Check(a, b)) {
				world.contactPairList[world.contactPairCount].a = a;
				world.contactPairList[world.contactPairCount++].b = b;

				//printf("contact count: %d\n", world.contactPairCount);				
			}
		}
	}
}
 
void Narrow_Phase()
{
	for (int i = 0; i < world.contactPairCount; i++) {

		Object* a = world.contactPairList[i].a;
		Object* b = world.contactPairList[i].b;
		
		vec2s normal = { 0.0f, 0.0f };
		float depth = 0.0f;
		if (Collider_SAT_Collision_Check(a, b, &normal, &depth)) {
			//printf("depth: %f, normal: {%f, %f}\n", depth, normal.x, normal.y);
			vec2s mtv = { normal.x * depth, normal.y * depth };
			SeperateBodies(a, b, mtv);
		}
			
		//if collide
			//seperate bodies
			//find contact points
			//resolve collisions
	}
}

void SeperateBodies(Object* a, Object* b, vec2s mtv) {
	if (a->isStatic) {
		b->position.x += mtv.x; b->position.y += mtv.y;

	}
	else if (b->isStatic) {
		a->position.x -= mtv.x; a->position.y -= mtv.y;
	}
	else {
		b->position.x += mtv.x / 2.0f; b->position.y += mtv.y / 2.0f;
		a->position.x -= mtv.x / 2.0f; a->position.y -= mtv.y / 2.0f;
	}
}