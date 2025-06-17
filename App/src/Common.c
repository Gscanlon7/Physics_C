#include "Common.h"

#include <malloc.h>

char* load_file(const char* path)
{
	FILE* file = fopen(path, "rb");
	if (!file) {
		printf("Failed to open file '%s'", path);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	long bytes = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = calloc(bytes + 1, sizeof(char));

	fread(buffer, sizeof(char), bytes, file);
	fclose(file);
	buffer[bytes] = '\0';

	return buffer;
}