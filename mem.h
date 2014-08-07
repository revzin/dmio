#ifndef _MEMORY_H_
#define _MEMORY_H_

void *MEM_malloc(size_t size, char *desc);
void *MEM_calloc(size_t size, char *desc);
void MEM_free(void *mem);

void MEM_printblocks(FILE *f);

int MEM_totblocks();
int MEM_totsize();

void MEM_do_guard();
void MEM_no_guard();

#endif
