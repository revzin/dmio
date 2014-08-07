#ifndef _GUID_H_
#define _GUID_H_

#ifndef WIN32
#error "UUID relies on rpc.h at the moment"
#endif

#include <rpc.h>

typedef struct guid_t
{
	char c[16];
} guid_t;


void create_guid(guid_t *guid);

void guid_to_sz(guid_t *from, char *to);

int sz_to_guid(char *from, guid_t *to);

#endif

