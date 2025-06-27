#pragma once

#include <cglm/cglm.h>
#include <cglm/struct.h>

void graphics_init();
void graphics_shutdown();

// VAO
uint32_t vao_create();
void vao_destroy(uint32_t* vao);
void vao_bind(uint32_t vao);

typedef struct
{
	uint32_t type;
	uint32_t count;
	uint32_t offset;
} VertexBufferLayoutElement;

// VBO
typedef struct
{
	uint32_t count;
	uint32_t stride;
	VertexBufferLayoutElement elements[8]; // max 8 elements, increase if u want
} VertexBufferLayout;

uint32_t vbo_create(void* vertices, size_t size);
void vbo_destroy(uint32_t* vbo);

uint32_t ibo_create(uint32_t* indices, uint32_t count);
void ibo_destroy(uint32_t* ibo);

void vao_set_ibo(uint32_t vao, uint32_t ibo);
void vao_add_vbo_with_layout(uint32_t vao, uint32_t vbo, VertexBufferLayout* layout);

// Shaders
uint32_t shader_load_and_create(const char* vertPath, const char* fragPath);

void shader_bind(uint32_t shader);
void shader_destroy(uint32_t shader);

uint32_t get_default_shader();

//void shader_set_int(const char* name,  int v);
//void shader_set_int2(const char* name, int2 v);
//void shader_set_int3(const char* name, int v);
//void shader_set_int4(const char* name, int v);

void shader_set_float(uint32_t shader, const char* name, float v);
void shader_set_float2(uint32_t shader, const char* name, vec2 v);
void shader_set_float3(uint32_t shader, const char* name, vec3 v);
void shader_set_float4(uint32_t shader, const char* name, vec4 v);

void shader_set_mat3(uint32_t shader, const char* name, mat3 v);
void shader_set_mat4(uint32_t shader, const char* name, mat4 v);

void graphics_update_camera(float x, float y, float zoom, float aspectRatio);
void graphics_draw_square(float x, float y, float width, float height, float zDeg, vec3s color);
void graphics_draw_circle(float x, float y, float radius, float zDeg, vec3s color);
//uint32_t load_texture(const char* filePath);