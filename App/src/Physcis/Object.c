#include "Object.h"
#include <memory.h>

void Object_Init(Object* obj) {
	memset(obj, 0, sizeof(Object));

	obj->collider.object = obj;

	obj->position = glms_vec2_zero();
	obj->mass = 1.0f;
	obj->rotation = 0.0f;
	obj->density = 1.0f;
	obj->restitution = 0.5f;
	obj->isStatic = 0;

	/*if (collider->density)
		collider->mass = Collider_Get_Area(collider) * collider->density;
	else if (collider->mass)
		collider->density = collider->mass / Collider_Get_Area(collider);
	else
		printf("Mass or density needs to be set before creating obj\n");*/
}


void Object_Step(Object* obj, float deltaTime, float gravity) {
	if (obj->isStatic)
		return;


	vec2s acceleration = { obj->force.x / obj->mass, obj->force.y / obj->mass };
	obj->linearVelocity.x += acceleration.x * deltaTime;
	obj->linearVelocity.y += acceleration.y * deltaTime;

	obj->linearVelocity.y += gravity * deltaTime;

	obj->position.x += obj->linearVelocity.x * deltaTime;
	obj->position.y += obj->linearVelocity.y * deltaTime;
	obj->rotation += obj->angularVelocity * deltaTime;

	obj->force = glms_vec2_zero();

}



void Object_Calculate_Inertia(Object* object) {
	if (object->collider.shape == Shape_Rect) {
		object->inertia = (1 / 12.0f)* object->mass* ((object->collider.vertices[1].x - object->collider.vertices[0].x) * (object->collider.vertices[1].x - object->collider.vertices[0].x)
			+ (object->collider.vertices[1].y - object->collider.vertices[2].y) * (object->collider.vertices[1].y - object->collider.vertices[2].y));
	}
}