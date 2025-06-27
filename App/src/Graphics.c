#include "Common.h"
#include "Graphics.h"

#include <glad/glad.h>

void opengl_message_callback(
	unsigned int source, unsigned int type, unsigned int id, unsigned int severity,
	int length, const char* message, const void* userParam)
{
	printf("OPENGL ERROR [severity = %d]\n", GL_DEBUG_SEVERITY_LOW - severity);
	printf("%s\n", message);

	ASSERT(0);
}

static uint32_t squareVAO, squareVBO, squareIBO;
static uint32_t circleVAO, circleVBO, circleIBO;
static uint32_t shader;

void graphics_init()
{
#ifdef APP_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(opengl_message_callback, 0);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, 0);
#endif
	{
		squareVAO = vao_create();
		float vertices[] =
		{
			-0.5f,  0.5f,
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
		};
		squareVBO = vbo_create(vertices, sizeof(vertices));
		VertexBufferLayout layout =
		{
			.elements =
			{
				{.type = GL_FLOAT, .count = 2 }
			},
		};
		layout.count = 1;
		vao_add_vbo_with_layout(squareVAO, squareVBO, &layout);

		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
		squareIBO = ibo_create(indices, 6);
		vao_set_ibo(squareVAO, squareIBO);
	}


	{

		//circle
		circleVAO = vao_create();
		const int resolution = 64;
		const size_t size = sizeof(vec2s) * resolution;
		vec2s* vertices = malloc(size);
		for (int i = 0; i < resolution; i++) {
			vertices[i].x = cos(glm_rad(i*360.0f/resolution));
			vertices[i].y = sin(glm_rad(i*360.0f/resolution));

		}

		circleVBO = vbo_create(vertices, size);
		VertexBufferLayout layout =
		{
			.elements =
			{
				{.type = GL_FLOAT, .count = 2 }
			},
		};
		layout.count = 1;
		vao_add_vbo_with_layout(circleVAO, circleVBO, &layout);
	}



	shader = shader_load_and_create("resources/vert.glsl", "resources/frag.glsl");
	shader_bind(shader);
}

void graphics_shutdown()
{
	vao_destroy(&squareVAO);
	vbo_destroy(&squareVBO);
	ibo_destroy(&squareIBO);
	shader_destroy(shader);
}

uint32_t vao_create()
{
	uint32_t vao = 0;
	glCreateVertexArrays(1, &vao);
	return vao;
}

void vao_bind(uint32_t vao)
{
	glBindVertexArray(vao);
}

void vao_destroy(uint32_t* vao)
{
	glDeleteVertexArrays(1, vao);
}

uint32_t vbo_create(void* vertices, size_t size)
{
	uint32_t vbo = 0;
	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, size, vertices, GL_STATIC_DRAW);

	return vbo;
}

void vbo_destroy(uint32_t* vbo)
{
	glDeleteBuffers(1, vbo);
}

uint32_t ibo_create(uint32_t* indices, uint32_t count)
{
	uint32_t ibo = 0;
	glCreateBuffers(1, &ibo);
	glNamedBufferData(ibo, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	return ibo;
}

void ibo_destroy(uint32_t* ibo)
{
	glDeleteBuffers(1, ibo);
}

void vao_set_ibo(uint32_t vao, uint32_t ibo)
{
	glVertexArrayElementBuffer(vao, ibo);
}

static uint32_t get_data_type_size(uint32_t type)
{
	switch (type)
	{
	case GL_FLOAT: return 4;
	case GL_INT: return 4;
	}

	ASSERT(0);
}

static void compute_vbo_layout(VertexBufferLayout* layout)
{
	uint32_t stride = 0;
	uint32_t offset = 0;
	for (uint32_t i = 0; i < layout->count; i++)
	{
		VertexBufferLayoutElement* element = &layout->elements[i];
		element->offset = offset;

		uint32_t type = element->type;
		uint32_t count = element->count;
		uint32_t size = get_data_type_size(type) * count;

		layout->stride += size;
		offset += size;
	}
}

void vao_add_vbo_with_layout(uint32_t vao, uint32_t vbo, VertexBufferLayout* layout)
{
	vao_bind(vao);

	uint32_t vboIndex = 0;

	compute_vbo_layout(layout);
	for (uint32_t i = 0; i < layout->count; i++)
	{
		VertexBufferLayoutElement* element = &layout->elements[i];

		glEnableVertexArrayAttrib(vao, vboIndex);
		glVertexArrayVertexBuffer(vao, vboIndex, vbo, element->offset, layout->stride);
		glVertexArrayAttribFormat(vao, vboIndex, element->count, element->type, 0, 0);
		vboIndex++;
	}
}

uint32_t shader_load_and_create(const char* vertPath, const char* fragPath)
{
	char* vertSource = load_file(vertPath);
	char* fragSource = load_file(fragPath);
	char* shaderSources[2] = { vertSource, fragSource };

	uint32_t program = glCreateProgram();
	uint32_t shaders[2];

	for (int i = 0; i < 2; i++)
	{
		uint32_t shader = glCreateShader(i == 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &shaderSources[i], 0);
		glCompileShader(shader);

		int compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			int maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			char* info = malloc(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, info);
			glDeleteShader(shader);

			printf("%s", info);
			ASSERT(0);
		}

		glAttachShader(program, shader);
		shaders[i] = shader;
	}

	// Link
	glLinkProgram(program);

	int linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char* info = malloc(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, info);
		glDeleteProgram(program);

		for (int i = 0; i < 2; i++)
			glDeleteShader(shaders[i]);

		printf("%s", info);
		ASSERT(0);

		return 0;
	}

	for (int i = 0; i < 2; i++)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	free(vertSource);
	free(fragSource);

	return program;
}

void shader_bind(uint32_t shader)
{
	glUseProgram(shader);
}

void shader_destroy(uint32_t shader)
{
	glDeleteProgram(shader);
}

uint32_t get_default_shader()
{
	return shader;
}

void shader_set_float(uint32_t shader, const char* name, float v)
{
	int location = glGetUniformLocation(shader, name);
	if (location == -1) return;

	glProgramUniform1f(shader, location, v);
}

void shader_set_float2(uint32_t shader, const char* name, vec2 v)
{
	int location = glGetUniformLocation(shader, name);
	if (location == -1) return;

	glProgramUniform2f(shader, location, v[0], v[1]);
}

void shader_set_float3(uint32_t shader, const char* name, vec3 v)
{
	int location = glGetUniformLocation(shader, name);
	if (location == -1) return;

	glProgramUniform3f(shader, location, v[0], v[1], v[2]);
}

void shader_set_float4(uint32_t shader, const char* name, vec4 v)
{
	int location = glGetUniformLocation(shader, name);
	if (location == -1) return;

	glProgramUniform4f(shader, location, v[0], v[1], v[2], v[3]);
}

void shader_set_mat3(uint32_t shader, const char* name, mat3 v)
{ 
	int location = glGetUniformLocation(shader, name);
	if (location == -1) return;

	glProgramUniformMatrix3fv(shader, location, 1, 0, v[0]);
}

void shader_set_mat4(uint32_t shader, const char* name, mat4 v)
{
	int location = glGetUniformLocation(shader, name);
	if (location == -1) return;

	glProgramUniformMatrix4fv(shader, location, 1, 0, v[0]); 
} 
 
void graphics_update_camera(float x, float y, float zoom, float aspectRatio)
{
	mat4 projection;
	glm_ortho(-zoom * aspectRatio, zoom * aspectRatio, -zoom, zoom, -1.0f, 1.0f, projection);

	mat4 view;
	glm_mat4_identity(view);
	vec3 pos = { x, y, 0.0f };
	glm_translate(view, pos);
	mat4 invView;
	glm_mat4_inv(view, invView);

	mat4 viewProj;
	glm_mul(projection, invView, viewProj);

	shader_set_mat4(get_default_shader(), "u_ViewProjection", viewProj);
}

void graphics_draw_square(float x, float y, float width, float height, float zDeg, vec3s color)
{
	mat4 transform;
	glm_mat4_identity(transform);

	vec3 position3d = { x, y, 0.0f };
	vec3 scale3d = { width, height, 1.0f };
	vec3 axis = { 0.0f, 0.0f, 1.0f };

	glm_translate(transform, position3d);
	glm_scale(transform, scale3d);
	glm_rotate(transform, glm_rad(zDeg), axis);

	shader_set_float3(shader, "u_Color", color.raw);
	shader_set_mat4(shader, "u_Transformation", transform);
	vao_bind(squareVAO);
	glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
}

void graphics_draw_circle(float x, float y, float radius, float zDeg, vec3s color)
{
	mat4 transform;
	glm_mat4_identity(transform);

	vec3 position3d = { x, y, 0.0f };
	vec3 scale3d = { radius, radius, 1.0f };
	vec3 axis = { 0.0f, 0.0f, 1.0f };

	glm_translate(transform, position3d);
	glm_scale(transform, scale3d);
	glm_rotate(transform, glm_rad(zDeg), axis);

	shader_set_float3(shader, "u_Color", color.raw);
	shader_set_mat4(shader, "u_Transformation", transform);
	
	vao_bind(circleVAO);
	
	glDrawArrays(GL_LINE_STRIP, 0, 2 * 360/8);
}

//uint32_t load_texture(const char* filePath)
//{
//	uint8_t*  image;
//
//	int width = 0, height = 0, channels = 0;
//	stbi_set_flip_vertically_on_load(1);
//	image = stbi_load(filePath, &width, &height, &channels, 0);
//	if (!image) {
//		printf("failed to load image file\n");
//		__debugbreak();
//	}
//
//	//glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
//	//glTextureStorage2D(m_ID, mips, (GLenum)format, m_Width, m_Height);
//
//	return image;
//	return 0;
//}
