#include "dmutil.h"

/* === simple linked list === */

typedef struct list_entry {
	void *next, *prev;
} list_entry;

int list_count(list *l)
{
	int a = 0;
	list_entry *iter;
	LIST_ITER(l, iter) {
		++a;
	}
	return a;
}

void list_append(list *l, void *lldata)
{
	list_entry *data = lldata;
	list_entry *iter = l->first;
	if (iter) {
		while (iter->next)
			iter = iter->next;
		iter->next = data;
		data->prev = iter;
		return;
	}
	else {
		l->first = data;
		data->prev = NULL;
	}
}

void list_remove(list *l, void *lldata)
{
	list_entry *iter;
	list_entry *data = lldata;

	if (!data)
		return;

	LIST_ITER(l, iter) {
		if (iter == data) {
			list_entry *prev = iter->prev;
			list_entry *next = iter->next;

			if (!next && !prev) {
				/* was single */
				l->first = NULL;
			} 
			else if (!next) {
				/* was at end */
				prev->next = NULL;
			}
			else if (!prev) {
				/* was at beginning */
				l->first = next;
				next->prev = NULL;
			} 
			else {
				prev->next = next;
				next->prev = prev;
			}
			return;
		}
	}
}

void *list_find(list *l, int index)
{
	list_entry *iter;
	
	if (index < 0)
		return NULL;

	LIST_ITER(l, iter) {
		if (!index)
			return iter;
		--index;
	}
	return NULL;
}

int list_index(list *l, void *lldata)
{
	list_entry *iter;
	list_entry *data = lldata;
	int a = 0;

	LIST_ITER(l, iter) {
		if (iter == data)
			return a;
		a++;
	}

	return -1;
}

void *list_pophead(list *l)
{
	list_entry *d = l->first;
	list_entry *next;
	
	if (!d)
		return NULL;

	assert(!d->prev);

	next = d->next;
	
	if (d->next) {
		next->prev = NULL;
	}

	l->first = next;
	d->next = NULL;
	return d;
}