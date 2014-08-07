#include "dmutil.h"
#include "dm.h"

#include "io_bin5.h"

const char *dmxFileHeader = "<!-- dmx encoding %s %d format %s %d -->\n";

size_t type_size_in_file(DMEAType t)
{
	switch (t) {
		case AT_ELEMENT:
		case AT_ELEMENT_ARRAY:
		case AT_INT:
		case AT_INT_ARRAY:
		case AT_FLOAT:
		case AT_FLOAT_ARRAY:
		case AT_VOID:		/* some binary data, size int */
		case AT_VOID_ARRAY:
		case AT_TIME:
		case AT_TIME_ARRAY:
		case AT_STRING:		/* it's an entry in the stringtable */
							/* stringarrays are stored in place!*/
			return sizeof(int);

		case AT_BOOL:
		case AT_BOOL_ARRAY:
			return 1;

		case AT_COLOR:
		case AT_COLOR_ARRAY:
			return sizeof(char[4]);

		case AT_VECTOR2:
		case AT_VECTOR2_ARRAY:
		case AT_VECTOR3:
		case AT_VECTOR3_ARRAY:
		case AT_VECTOR4:
		case AT_VECTOR4_ARRAY:
		case AT_VMATRIX:
		case AT_VMATRIX_ARRAY:
		case AT_QUATERNION:
		case AT_QUATERNION_ARRAY:
		case AT_QANGLE:
		case AT_QANGLE_ARRAY:
			return sizeof(float) * DM_components_in_type(t);
		
		case AT_STRING_ARRAY:
			printf("%s\n", "Stringarrays are stored in-place!");
			assert(0);
	}
	assert(0);
	return 0;
}

typedef struct ioDMEAttribute 
{
	int name;
	DMEAType type;
	void *data;
} ioDMEAttribute;

typedef struct ioDMEHeader
{
	int type;
	int name; /* string table */
	guid_t *guid;
} ioDMEHeader;

typedef struct ioDMEBody
{
	int totattr;
	ioDMEAttribute *attrs;
} ioDMEBody;

typedef struct ioDM
{
	char *header;

	int totstring;
	char *strings; /* packed null-terminated string table */

	int totelem;
	ioDMEHeader *elementHeaders;
	ioDMEBody *elementBodies;
};


ValveDataModel *DMX_Bin5_Read(FILE *f);
void		    DMX_Bin5_Write(ValveDataModel *dm);

