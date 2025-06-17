#pragma once

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#ifdef APP_DEBUG
	#define ASSERT(x) { if (!(x)) { __debugbreak(); } }
#else
	#define ASSERT(x) (x)
#endif

char* load_file(const char* path);