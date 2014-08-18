#ifndef _DMUTIL_H_
#define _DMUTIL_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#if defined ( WIN32 )
#define __func__ __FUNCTION__
#endif

#define STATIC_ASSERT(cond) typedef int foo[(cond) ? 1 : -1]

/* check we aren't on some weird platform where a float won't fit 
 * in a pointer's place */
STATIC_ASSERT(sizeof(float) <= sizeof(float *));

/* simple linked list */

#define LIST_LINKS void *next, *prev

typedef struct list {
	void *first;
} list;

#define LIST_ITER(list, iter) \
for (iter = (list)->first; iter; (iter) = (iter)->next)

int list_count(list *l);

void list_append(list *l, void *lldata);

void list_remove(list *l, void *lldata);

void *list_find(list *l, int index);

int list_index(list *l, void *lldata);

int list_has_entry(list *l, void *lldata);

void *list_pophead(list *l);


/* static memory description strings are wrapped in that, to reduce bin size */
#ifdef _DEBUG
#define _MD(x) x
#else
#define _MD(x) NULL
#endif


#endif _DMUTIL_H_
