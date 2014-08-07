#ifndef _DM_H_
#define _DM_H_

#include "guid.h"
#include "valvetypes.h"

/* this is a table of stuff that what can we store in our datamodel 
 * for each attribute type, there's a setter and a getter for the value*/
typedef enum DMEAType {
	AT_UNKNOWN = 0,

	AT_FIRST_VALUE_TYPE,

	AT_ELEMENT = AT_FIRST_VALUE_TYPE,
	AT_INT,
	AT_FLOAT,
	AT_BOOL,
	AT_STRING,
	AT_VOID,
	AT_TIME,
	AT_COLOR, 
	AT_VECTOR2,
	AT_VECTOR3,
	AT_VECTOR4,
	AT_QANGLE,
	AT_QUATERNION,
	AT_VMATRIX,

	AT_FIRST_ARRAY_TYPE,

	AT_ELEMENT_ARRAY = AT_FIRST_ARRAY_TYPE,
	AT_INT_ARRAY,
	AT_FLOAT_ARRAY,
	AT_BOOL_ARRAY,
	AT_STRING_ARRAY,
	AT_VOID_ARRAY,
	AT_TIME_ARRAY,
	AT_COLOR_ARRAY,
	AT_VECTOR2_ARRAY,
	AT_VECTOR3_ARRAY,
	AT_VECTOR4_ARRAY,
	AT_QANGLE_ARRAY,
	AT_QUATERNION_ARRAY,
	AT_VMATRIX_ARRAY,
	AT_TYPE_COUNT,

	AT_TYPE_INVALID,
} DMEAType;

typedef struct DMEAValue 
{
	int totelem;	/* -1 if it's not an array */
	void *data;		/* this is used differently by different types of attributes 
					 * 1) if data fits into a 32-bit pointer (4 bytes), store exactly here 
					 * 2) if it's a single value, this points to a buffer where the value can be stored
					 *     ther buffer gets allocated on element creation and deallocated on element free
	                 * 3) if it's an array, it's a pointer to it and totelem is the total number 
					 * don't access directly, bad things will happen */
	/* note: datamodel always stores its own copy of any data, and will do all allocations and frees itself! */
} DMEAValue;

#define DME_ATTRIBUTE_MAX_NAME 512
#define DM_ELEMENT_MAX_NAME 512
#define DM_ELEMENT_MAX_TYPE 512

typedef struct DMEAttribute  {
	struct DMEAttribute *next, *prev;

	char *name;
	DMEAType type;		/* private, changing an attribute's type is forbidden */
	DMEAValue val;		
} DMEAttribute;

typedef struct DMElement {
	struct DMElement *next, *prev;

	struct ValveDataModel *dm; /* where did I come from? */
	
	char *type;
	char *name;
	guid_t guid; /* every element has an unuque GUID */

	list attrs; /* linked list of attributes of this element */
} DMElement;

typedef struct ValveDataModel {
	char *format;		/* examples of format: "model", "sfm_session", "preset"...
						 * tells us whatever is stored in the datamodel */
	int format_version; /* formats can have versions */

	list elems; /* linked list of DMElements */
} ValveDataModel;


void DM_Init(ValveDataModel *dm, char *format, int format_version);
void DM_Release(ValveDataModel *dm);

/* adds an element to the datamodel */
DMElement *DM_add_element(ValveDataModel *dm, char *name, char *type);
/* removes an element from the datamodel */
void DM_remove_element(ValveDataModel *dm, DMElement *ele);

/* adds an attribute to the element */
DMEAttribute *DME_add_attribute(DMElement *ele, char *name, DMEAType type);
/* removes an attribute from the element */
void DME_remove_attribute(DMElement *ele, DMEAttribute *attr);
/* finds an attribute on the element by name, skip tells how many attributes to skip */
DMEAttribute *DME_find_attribute(DMElement *ele, char *name, int skip);

/* find element with that name in the datamodel */
DMElement *DM_find_element(ValveDataModel *dm, char *name, DMElement *prev, int skip);

/* find first child element with that name, does a recursive search down the tree
 * depth = 0 checks for the element only on immidiate children */
DMElement *DM_find_child(DMElement *ele, char *name, int depth);

/* returns the length of the value array, returns -1 if not array type */
int DMEA_ArrayLen(DMEAttribute *a);

/* attribute setters and getters */
void DMEA_SetElem(DMEAttribute *a, DMElement *ele);
DMElement *DMEA_GetElem(DMEAttribute *a);

int DMEA_GetInt(DMEAttribute *a);
void DMEA_SetInt(DMEAttribute *a, int val);

int DMEA_GetVoid(DMEAttribute *a);
void DMEA_SetVoid(DMEAttribute *a, int val);

float DMEA_GetFloat(DMEAttribute *a);
void DMEA_SetFloat(DMEAttribute *a, float val);

VTimeValue DMEA_GetTime(DMEAttribute *a);
void DMEA_SetTime(DMEAttribute *a, VTimeValue val);

char DMEA_GetBool(DMEAttribute *a);
void DMEA_SetBool(DMEAttribute *a, char val);

void DMEA_SetVec3(DMEAttribute *a, float vec[3]);
void DMEA_GetVec3(DMEAttribute *a, float vec[3]);

void DMEA_SetVec2(DMEAttribute *a, float vec[2]);
void DMEA_GetVec2(DMEAttribute *a, float vec[2]);

void DMEA_SetVec4(DMEAttribute *a, float vec[4]);
void DMEA_GetVec4(DMEAttribute *a, float vec[4]);

void DMEA_GetQuat(DMEAttribute *a, VQuat *q);
void DMEA_SetQuat(DMEAttribute *a, VQuat *q);

void DMEA_GetQAngles(DMEAttribute *a, VQAngles *angles);
void DMEA_SetQAngles(DMEAttribute *a, VQAngles *angles);

void DMEA_SetVMatrix(DMEAttribute *a, VMatrix *mtx);
void DMEA_GetVMatrix(DMEAttribute *a, VMatrix *mtx);

void DMEA_SetVColor(DMEAttribute *a, VColor *rgba);
void DMEA_GetVColor(DMEAttribute *a, VColor *rgba);

void DMEA_SetString(DMEAttribute *a, char *str);
void DMEA_GetString(DMEAttribute *a, char **str);

/* arrays */
/* to know how long is the array you've got, use DMEA_ArrayLen */
void DMEA_SetIntArray(DMEAttribute *a, int data[], int tot);
void DMEA_GetIntArray(DMEAttribute *a, int *data[]);

void DMEA_SetFloatArray(DMEAttribute *a, float data[], int tot);
void DMEA_GetFloatArray(DMEAttribute *a, float *data[]);

void DMEA_SetBoolArray(DMEAttribute *a, char data[], int tot);
void DMEA_GetBoolArray(DMEAttribute *a, char *data[]);

void DMEA_SetVoidArray(DMEAttribute *a, int data[], int tot);
void DMEA_GetVoidArray(DMEAttribute *a, int *data[]);

void DMEA_SetTimeArray(DMEAttribute *a, VTimeValue data[], int tot);
void DMEA_GetTimeArray(DMEAttribute *a, VTimeValue *data[]);

void DMEA_SetColorArray(DMEAttribute *a, VColor data[], int tot);
void DMEA_GetColorArray(DMEAttribute *a, VColor *data[]);

void DMEA_SetVec3Array(DMEAttribute *a, VVec3 data[], int tot);
void DMEA_GetVec3Array(DMEAttribute *a, VVec3 *data[]); 

void DMEA_SetVec2Array(DMEAttribute *a, VVec2 data[], int tot);
void DMEA_GetVec2Array(DMEAttribute *a, VVec2 *data[]);

void DMEA_SetVec4Array(DMEAttribute *a, VVec4 data[], int tot);
void DMEA_GetVec4Array(DMEAttribute *a, VVec4 *data[]);

void DMEA_SetQAngleArray(DMEAttribute *a, VQAngles data[], int tot);
void DMEA_GetQAngleArray(DMEAttribute *a, VQAngles *data[]);

void DMEA_SetQuatArray(DMEAttribute *a, VQuat data[], int tot);
void DMEA_GetQuatArray(DMEAttribute *a, VQuat *data[]);

void DMEA_SetMtxArray(DMEAttribute *a, VMatrix data[], int tot);
void DMEA_GetMtxArray(DMEAttribute *a, VMatrix *data[]);

/* don't get confused by this, string arrays are treated similiar to argc and argv in your 
 * int main(int argc, char **argv) :)*/
void DMEA_SetStringArray(DMEAttribute *a, char *strings[], int tot);
char **DMEA_GetStringArray(DMEAttribute *a);

int DM_components_in_type(DMEAType t);
#endif // _DM_H_
