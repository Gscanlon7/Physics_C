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



void Object_Calculate_Inertia(Object* obj) {
	if (obj->collider.shape == Shape_Rect) {
		obj->inertia = (1 / 12.0f)* obj->mass* ((obj->collider.vertices[1].x - obj->collider.vertices[0].x) * (obj->collider.vertices[1].x - obj->collider.vertices[0].x)
			+ (obj->collider.vertices[1].y - obj->collider.vertices[2].y) * (obj->collider.vertices[1].y - obj->collider.vertices[2].y));
	}
}




vec2s* Object_Transform_Vertices(Object* obj) {
	vec2s* transformedVertices = malloc(obj->collider.vertexCount * sizeof(vec2s));
	memccpy(transformedVertices, obj->collider.vertices, 0, obj->collider.vertexCount * sizeof(vec2s));
	for (int i = 0; i < obj->collider.vertexCount; i++) {
		transformedVertices[i].x += obj->position.x;
		transformedVertices[i].y += obj->position.y;
	}
	return transformedVertices;
}