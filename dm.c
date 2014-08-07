#include <stdlib.h>
#include <assert.h>

#include "dmutil.h"
#include "mem.h"

#include "dm.h"

static char *dupstring(char *string, char *desc)
{
	int size = strlen(string) + 1;
	char *new;

	assert(string);
	new = MEM_malloc(size, desc);
	strncpy(new, string, size);
	return new;
}


static size_t type_to_elemsize(DMEAType t)
{	
	switch (t) {
		case AT_ELEMENT:
		case AT_ELEMENT_ARRAY:
		case AT_INT:
		case AT_INT_ARRAY:
		case AT_FLOAT:
		case AT_FLOAT_ARRAY:
		case AT_VOID:		
		case AT_VOID_ARRAY:
		case AT_TIME:
		case AT_TIME_ARRAY:	
			return 4;

		case AT_BOOL:
		case AT_BOOL_ARRAY:
			return 1;


		case AT_COLOR:
		case AT_COLOR_ARRAY:
			return sizeof(VColor);

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
		
		case AT_STRING:
		case AT_STRING_ARRAY:
			printf("%s\n", "String len is undefined!");
			assert(0);
	}
	assert(0);
	return 0;
}


/* ====================== Setters and getters =========================== */

/* in-place types, stored right at DMA->data */

/* elements are stored as pointer to the element! */
void DMEA_SetElem(DMEAttribute *a, DMElement *ele)
{
	assert(a->type == AT_ELEMENT);
	a->val.data = (void *) ele;
}

DMElement *DMEA_GetElem(DMEAttribute *a)
{
	assert(a->type == AT_ELEMENT);
	return a->val.data;
}

int DMEA_GetInt(DMEAttribute *a)
{
	assert(a->type == AT_INT);
	return (int) a->val.data;
}

void DMEA_SetInt(DMEAttribute *a, int val)
{
	assert(a->type == AT_INT);
	a->val.data = (void *)val;
}

int DMEA_GetVoid(DMEAttribute *a)
{
	assert(a->type == AT_VOID);
	return (int) a->val.data;
}

void DMEA_SetVoid(DMEAttribute *a, int val)
{
	assert(a->type == AT_VOID);
	a->val.data = (void *)val;
}

float DMEA_GetFloat(DMEAttribute *a)
{
	float r;
	assert(a->type == AT_FLOAT);
	memcpy(&r, &a->val.data , sizeof(float));
	return r;
}

void DMEA_SetFloat(DMEAttribute *a, float val)
{
	assert(a->type == AT_FLOAT);
	memcpy(&a->val.data, &val, sizeof(float));
}

VTimeValue DMEA_GetTime(DMEAttribute *a)
{
	VTimeValue r;
	assert(a->type == AT_TIME);
	memcpy(&r, &a->val.data, sizeof(VTimeValue));
	return r;
}

void DMEA_SetTime(DMEAttribute *a, VTimeValue val)
{
	assert(a->type == AT_TIME);
	memcpy(&a->val.data, &val, sizeof(VTimeValue));
}

char DMEA_GetBool(DMEAttribute *a)
{
	assert(a->type == AT_BOOL);
	return (char) a->val.data;
}

void DMEA_SetBool(DMEAttribute *a, char val)
{
	assert(a->type == AT_BOOL);
	assert(val == 1 || val == 0);
	a->val.data = (void *)val;
}


/* out-of-place types, stored by pointer that is DMA->data */
/* for these types, ALWAYS make sure the space is allocated on attribute creation */
void DMEA_SetVec3(DMEAttribute *a, float vec[3])
{
	assert(a->type == AT_VECTOR3);
	memcpy(a->val.data, vec, sizeof(float) * 3);
}

void DMEA_GetVec3(DMEAttribute *a, float vec[3])
{
	assert(a->type == AT_VECTOR3);
	memcpy(vec, a->val.data, sizeof(float) * 3);
}

void DMEA_SetVec2(DMEAttribute *a, float vec[2])
{
	assert(a->type == AT_VECTOR2);
	memcpy(a->val.data, vec, sizeof(float) * 2);
}

void DMEA_GetVec2(DMEAttribute *a, float vec[2])
{
	assert(a->type == AT_VECTOR2);
	memcpy(vec, a->val.data, sizeof(float) * 2);
}

void DMEA_SetVec4(DMEAttribute *a, float vec[4])
{
	assert(a->type == AT_VECTOR4);
	memcpy(a->val.data, vec, sizeof(float) * 4);
}

void DMEA_GetVec4(DMEAttribute *a, float vec[4])
{
	assert(a->type == AT_VECTOR4);
	memcpy(vec, a->val.data, sizeof(float) * 4);
}

void DMEA_GetQuat(DMEAttribute *a, VQuat *q)
{
	assert(a->type == AT_QUATERNION);
	memcpy(q, a->val.data, sizeof(VQuat));
}

void DMEA_SetQuat(DMEAttribute *a, VQuat *q)
{
	assert(a->type == AT_QUATERNION);
	memcpy(a->val.data, q, sizeof(VQuat));
}

void DMEA_GetQAngles(DMEAttribute *a, VQAngles *angles)
{
	assert(a->type == AT_QANGLE);
	memcpy(angles, a->val.data, sizeof(VQAngles));
}

void DMEA_SetQAngles(DMEAttribute *a, VQAngles *angles)
{
	assert(a->type == AT_QANGLE);
	assert(a && angles);
	memcpy(a->val.data, angles, sizeof(VQAngles));
}

void DMEA_SetVMatrix(DMEAttribute *a, VMatrix *mtx)
{
	assert(a->type == AT_VMATRIX);
	assert(a && mtx);
	memcpy(a->val.data, mtx, sizeof(VMatrix));
}

void DMEA_GetVMatrix(DMEAttribute *a, VMatrix *mtx)
{
	assert(a->type == AT_VMATRIX);
	assert(a && mtx);

	memcpy(mtx, a->val.data, sizeof(float) * 16);
}

void DMEA_SetVColor(DMEAttribute *a, VColor *rgba)
{
	assert(a->type == AT_COLOR);
	assert(a && rgba);
	memcpy(a->val.data, rgba, sizeof(VColor));
}

void DMEA_GetVColor(DMEAttribute *a, VColor *rgba)
{
	assert(a->type == AT_COLOR);
	assert(a && rgba);
	memcpy(rgba, a->val.data, sizeof(VColor));
}

/* crazy string type */
void DMEA_SetString(DMEAttribute *a, char *str)
{	
	assert(a->type == AT_STRING);
	assert(a && str);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = dupstring(str, "datamodel attribute string");
}

void DMEA_GetString(DMEAttribute *a, char **str) 
{
	assert(a && str);
	assert(a->type == AT_STRING);

	*str = dupstring(a->val.data, "string");
}

/* arrays */

static void *set_array_data(DMEAType t, void *array_data, int tot, char *typename)
{
	void *data = MEM_calloc(type_to_elemsize(t) * tot, typename);
	assert(data);
	memcpy(data, array_data, type_to_elemsize(t) * tot);
	return data;
}

void DMEA_SetIntArray(DMEAttribute *a, int data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_INT_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_INT_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetIntArray(DMEAttribute *a, int *data[])
{
	assert(a);
	assert(a->type == AT_INT_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "int array");
}

void DMEA_SetFloatArray(DMEAttribute *a, float data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_FLOAT_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_FLOAT_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetFloatArray(DMEAttribute *a, float *data[])
{
	assert(a);
	assert(a->type == AT_FLOAT_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "float array");
}

void DMEA_SetBoolArray(DMEAttribute *a, char data[], int tot)
{
	int b;
	assert(a && data && tot > 0);
	assert(a->type == AT_BOOL_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	for (b = 0; b < tot; ++b)
		assert(data[b] == 0 || data[b] == 1);

	a->val.data = set_array_data(a->type, data, tot, "AT_BOOL_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetBoolArray(DMEAttribute *a, char *data[])
{
	assert(a);
	assert(a->type == AT_BOOL_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "booleans array");
}

void DMEA_SetVoidArray(DMEAttribute *a, int data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_VOID_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_VOID_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetVoidArray(DMEAttribute *a, int *data[])
{
	assert(a);
	assert(a->type == AT_VOID_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "'void' int array");
}

void DMEA_SetTimeArray(DMEAttribute *a, VTimeValue data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_TIME_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_TIME_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetTimeArray(DMEAttribute *a, VTimeValue *data[])
{
	assert(a);
	assert(a->type == AT_TIME_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "'time' float array");
}

void DMEA_SetColorArray(DMEAttribute *a, VColor data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_COLOR_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_COLOR_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetColorArray(DMEAttribute *a, VColor *data[])
{
	assert(a);
	assert(a->type == AT_COLOR_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "VColor rgba int array");
}

void DMEA_SetVec3Array(DMEAttribute *a, VVec3 data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_VECTOR3_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_VECTOR3_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetVec3Array(DMEAttribute *a, VVec3 *data[])
{
	assert(a);
	assert(a->type == AT_VECTOR3_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "vec3 float array");
}

void DMEA_SetVec2Array(DMEAttribute *a, VVec2 data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_VECTOR2_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_VECTOR2_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetVec2Array(DMEAttribute *a, VVec2 *data[])
{
	assert(a);
	assert(a->type == AT_VECTOR2_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "vec2 float array");
}

void DMEA_SetVec4Array(DMEAttribute *a, VVec4 data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_VECTOR4_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_VECTOR4_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetVec4Array(DMEAttribute *a, VVec4 *data[])
{
	assert(a);
	assert(a->type == AT_VECTOR4_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "vec4 float array");
}

void DMEA_SetQAngleArray(DMEAttribute *a, VQAngles data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_QANGLE_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_QANGLE_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetQAngleArray(DMEAttribute *a, VQAngles *data[])
{
	assert(a);
	assert(a->type == AT_QANGLE_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "qangles float[3] array");
}

void DMEA_SetQuatArray(DMEAttribute *a, VQuat data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_QUATERNION_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_QUATERNION_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetQuatArray(DMEAttribute *a, VQuat *data[])
{
	assert(a);
	assert(a->type == AT_QUATERNION_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "quat float[4] array");
}

void DMEA_SetMtxArray(DMEAttribute *a, VMatrix data[], int tot)
{
	assert(a && data && tot > 0);
	assert(a->type == AT_VMATRIX_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, data, tot, "AT_VMATRIX_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetMtxArray(DMEAttribute *a, VMatrix *data[])
{
	assert(a);
	assert(a->type == AT_VMATRIX_ARRAY);
	*data = set_array_data(a->type, a->val.data, a->val.totelem, "VMATRIX float[16] array");
}

void DMEA_SetElementArray(DMEAttribute *a, DMElement *elems[], int tot)
{
	assert(a && elems && tot > 0);
	assert(a->type == AT_ELEMENT_ARRAY);

	if (a->val.data)
		MEM_free(a->val.data);

	a->val.data = set_array_data(a->type, elems, tot, "AT_VMATRIX_ARRAY");
	a->val.totelem = tot;
}

void DMEA_GetElementArray(DMEAttribute *a, DMElement *elems[])
{
	assert(a);
	assert(a->type == AT_ELEMENT_ARRAY);
	*elems = set_array_data(a->type, a->val.data, a->val.totelem, "DMElement pointers array");
}

static void free_stringarray(char **strings, int totstring)
{
	int b;
	for (b = 0; b < totstring; ++b)
		MEM_free(strings[b]);

	MEM_free(strings);
}

/* string arrays are the worst thing ever, handle them similar to argc, argv :) */
void DMEA_SetStringArray(DMEAttribute *a, char **strings, int tot)
{
	int b;
	char **dmstrings = NULL;
	assert(a && strings && tot > 0);
	assert(a->type == AT_STRING_ARRAY);

	if (a->val.data) 
		free_stringarray((char **) a->val.data, a->val.totelem);
	

	/* allocate tot string pointers */
	dmstrings = a->val.data = MEM_malloc(tot * sizeof(char *), "AT_STRING_ARRAY");

	for (b = 0; b < tot; b++) {
		/* for each string, alloc and copy */
		dmstrings[b] = dupstring(strings[b], "datamodel string array string");
	}

	a->val.totelem = tot;
}

char **DMEA_GetStringArray(DMEAttribute *a)
{
	int b;
	char **outstrings = NULL;
	char **dmstrings = a->val.data;
	assert(a);
	assert(a->type == AT_STRING_ARRAY);

	if (a->val.totelem) {

		/* allocate tot string pointers */
		outstrings = MEM_malloc(a->val.totelem * sizeof(char *), "string array");

		for (b = 0; b < a->val.totelem; b++) {
			/* for each string, alloc and copy */
			outstrings[b] = dupstring(dmstrings[b], "stringarray string");
		}

		return outstrings;
	}
	else {
		return NULL;
	}
}


/* ================= Attribute stuff ================== */


DMEAttribute *DME_add_attribute(DMElement *ele, char *name, DMEAType type)
{
	DMEAttribute *a = MEM_calloc(sizeof(DMEAttribute), "datamodel element attribute");

	assert(ele && name);

	if (!a)
		return NULL;

	assert(type < AT_TYPE_COUNT && type > 0);
	a->type = type;

	a->name = dupstring(name, "datamodel element attribute name");

	if (a->type < AT_FIRST_ARRAY_TYPE && a->type != AT_STRING) {
		int size = type_to_elemsize(a->type);
		if (size <= sizeof(void *)) {
			/* fall-through, the value will be right at the pointer */
		}
		else {
			/* allocate a buffer for the value */
			a->val.data = MEM_calloc(size, "datamodel element attribute value buffer");
		}
	}

	list_append(&ele->attrs, a);

	/* if it's an array attribute or a string, getters and setters take care of memory */
	return a;
}

static void remove_attr_internal(DMElement *ele, DMEAttribute *a)
{
	if (a->type < AT_FIRST_ARRAY_TYPE && a->type != AT_STRING) {
		int size = type_to_elemsize(a->type);
		if (size <= sizeof(void *)) {
			/* fall-through, the value was at the pointer, no need to free anything */
		}
		else {
			/* free the value buffer */
			MEM_free(a->val.data);
		}
	}
	else {
		/* need to free the array or the string */
		if (a->val.data) {
			if (a->type == AT_STRING_ARRAY)
				free_stringarray((char **)a->val.data, a->val.totelem);
			else
				MEM_free(a->val.data);
		}
	}

	list_remove(&ele->attrs, a);

	MEM_free(a->name);
}

void DME_remove_attribute(DMElement *ele, DMEAttribute *a)
{
	assert(ele && a);

	/* check the attribute is there */
	if (list_index(&ele->attrs, a) == -1) {
		printf("%s\n", "Warning: trying to remove an attribute that never existed!");
		return;
	}

	remove_attr_internal(ele, a);
	MEM_free(a);
}


/* =========================== element stuff ======================= */

DMElement *DM_add_element(ValveDataModel *dm, char *name, char *type)
{
	DMElement *new = MEM_calloc(sizeof(DMElement), "datamodel element");

	assert(new && name && type);

	new->name = dupstring(name, "datamodel element name string");
	new->type = dupstring(type, "datamodel element type string");

	list_append(&dm->elems, new);

	return new;
}


void DM_remove_element(ValveDataModel *dm, DMElement *ele)
{
	DMEAttribute *a;

	assert(dm && ele);

	if (list_index(&dm->elems, ele) == -1) {
		printf("%s\n", "Warning: trying to remove an element that never existed on the datamodel!");
		return;
	}
	
	while (a = list_pophead(&ele->attrs)) {
		remove_attr_internal(ele, a);
		MEM_free(a);
	}
	
	list_remove(&dm->elems, ele);

	MEM_free(ele->name);
	MEM_free(ele->type);
	MEM_free(ele);
}


void DM_Init(ValveDataModel *dm, char *format, int format_version)
{
	memset(dm, 0, sizeof(ValveDataModel));

	dm->format = dupstring(format, "datamodel format string");
	dm->format_version = format_version;
}


void DM_Release(ValveDataModel *dm) 
{
	DMElement *ele;
	while (ele = list_pophead(&dm->elems)) {
		DM_remove_element(dm, ele);
	}
	MEM_free(dm->format);
}


/* ========================================================================= */


int DM_components_in_type(DMEAType t)
{
	assert(t > 0 && t < AT_TYPE_COUNT);
	if (t >= AT_FIRST_ARRAY_TYPE) {
		t -= (AT_FIRST_ARRAY_TYPE - AT_FIRST_VALUE_TYPE);
	}
	switch (t)
		{
		case AT_BOOL:
		case AT_INT:
		case AT_FLOAT:
		case AT_TIME:
			return 1;

		case AT_VECTOR2:
			return 2;

		case AT_VECTOR3:
		case AT_QANGLE:
			return 3;

		case AT_COLOR:
		case AT_VECTOR4:
		case AT_QUATERNION:
			return 4;

		case AT_VMATRIX:
			return 16;

		case AT_ELEMENT:
		case AT_STRING:
		case AT_VOID:
		default:
			return 0;
	}
}

int DMEA_ArrayLen(DMEAttribute *a)
{
	if (a->type >= AT_FIRST_ARRAY_TYPE)
		return a->val.totelem;
	else return -1;
}

