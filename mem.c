#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dmutil.h"

typedef struct block
{
	struct block *next, *prev;
	void *data;
	size_t size;
	char *description;
} block;

typedef struct mem
{
	int totblocks;
	int totsize;
	int guard;

	list blocks;
} mem;

static mem Memory = {0};

void *MEM_malloc(size_t size, char *desc)
{
	block *memblock = calloc(1, sizeof(block));

	if (Memory.guard) {
		memblock->size = size;
		memblock->description = desc;
		memblock->data = malloc(size);
		list_append(&Memory.blocks, memblock);
		Memory.totblocks++;
		Memory.totsize += size;
		return memblock->data;
	}
	else
		return malloc(size);
	
}

void *MEM_calloc(size_t size, char *desc)
{
	void *data = MEM_malloc(size, desc);
	memset(data, 0, size);
	return data;
}

void MEM_free(void *mem)
{
	if (Memory.guard) {
		block *iter;
		list *l = &Memory.blocks;

		if (!mem) {
			printf("%s\n", "Attempt to free NULL memory!");
			return;
		}

		LIST_ITER(l, iter) {
			if (mem == iter->data) {
				free(mem);
				mem = NULL;			/* signal that we freed */
				Memory.totblocks--;
				Memory.totsize -= iter->size;
				list_remove(l, iter);
				free(iter);
				break;
			}
		}

		if (mem) {
			printf("%s\n", "Warning: attempt to free un-allocated memory.");
			return;
		}

	}
	else
		free(mem);
}

static const char bytes[] = " B";
static const char kbytes[] = "KB";
static const char mbytes[] = "MB";
static const char gbytes[] = "GB";

static const char *normalize_size(size_t size, float *out_size) 
{
	if (size < 1024) {
		return bytes;
	}
	if (size < 1024 * 1024)
		*out_size = ((float) size) / 1024.0f;
		return kbytes;
	if (size < 1024 * 1024 * 1024) {
		*out_size = ((float) size) / 1024.0f / 1024.0f;
		return mbytes;
	}
	else
		*out_size = ((float) size) / 1024.0f / 1024.0f / 1024.0f;
		return gbytes;

	return NULL;
}

/* ugleh */
void MEM_printblocks(FILE *f)
{
	if (Memory.guard) {
		block *iter;
		list *l = &Memory.blocks;
		printf("%s\n", " ========== Memory print ==========");
		printf("%s\n", "Size \t  \t Description");
		LIST_ITER(l, iter) {
			size_t size = iter->size;
			if (size < 1024)
				printf("%d \t %s: \"%s\" \n", iter->size, bytes, iter->description);
			else {
				float nsize;
				const char *size_unit = normalize_size(size, &nsize);
				printf("%.2f \t %s: \"%s\" \n", nsize, size_unit, iter->description);
			}
		}
		if (Memory.totsize < 1024)
			printf("Total: %d memory blocks over %d %s.\n", Memory.totblocks, bytes, Memory.totsize);
		else {
			float nsize;
			const char *size_unit = normalize_size(Memory.totsize, &nsize);
			printf("Total: %d memory blocks over %.2f %s.\n", Memory.totblocks, nsize, size_unit);
		}
		printf("%s\n", " ==================================");
	}
}

int  MEM_totblocks()
{
	return Memory.totblocks;
}

int  MEM_totsize()
{
	return Memory.totsize;
}

void MEM_do_guard()
{
	Memory.guard = 1;
}

void MEM_no_guard()
{
	Memory.guard = 0;
}
