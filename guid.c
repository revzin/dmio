#include <rpc.h>

#include "guid.h"

void create_guid(guid_t *guid)
{
	UuidCreate((UUID *) guid);
}

void guid_to_sz(guid_t *from, char *to)
{
	UuidToStringA((const UUID *)from, &to);
}

int sz_to_guid(char *from, guid_t *to)
{
	if (RPC_S_OK != UuidFromStringA(from, (UUID *) to)) {
		return 0;
	}
	return 1;
}
