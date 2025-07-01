#pragma once
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include "collider.h"


struct Object {

	vec2s position;
	vec2s linearVelocity;
	vec2s force;

	float angularVelocity;
	float rotation;

	float inertia;
	float density;
	float restitution;
	float mass;
	int isStatic;
	//enum Layer layer;

	Collider collider;
};
void Object_Init(Object* obj);
void Object_Calculate_Inertia(Object* obj);
void Object_Step(Object* obj, float deltaTime, float gravity);
vec2s* Object_Transform_Vertices(Object* obj);