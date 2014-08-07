#include <stdio.h>
#include <conio.h>

#include "dmutil.h"
#include "dm.h"
#include "mem.h"

//#define TEST_MEM
#define TEST_DM
int main(int argc, char **argv)
{
	MEM_do_guard();
	
#ifdef TEST_DM
	DMElement *ele;
	
	DMEAttribute *attr;
	DMEAttribute *ints;
	DMEAttribute *vec4;

	ValveDataModel dm;

	char *s1 = "Wazzup!";
	char *s2;

	int vec[3] = {1, 2, 3};
	int *vi;
	float v4[4] = { 4, 3, 2, 1 };

	char *sar1 = "Yah!";
	char *sar2 = "Wooh!";
	char *sar3 = "Bar!";

	char *sar[3];
	sar[0] = sar1;
	sar[1] = sar2;
	sar[2] = sar3;

	char **sar_back;

	VMatrix *vm = MEM_calloc(sizeof(VMatrix) * 1000, "test matrices");
	VMatrix *back;

	for (int c = 0; c < 1000; ++c)
		for (int a = 0; a < 4; ++a)
			for (int b = 0; b < 4; ++b)
				vm[c].m[a][b] = (float) c - 2 + a + b - a * b;

	DM_Init(&dm, "test", 0);

	ele = DM_add_element(&dm, "element-name", "element-type");

	MEM_printblocks(stdout);

	attr = DME_add_attribute(ele, "FIRST", AT_FLOAT);
	DMEA_SetFloat(attr, 1.3f);
	
	ints = DME_add_attribute(ele, "SECOND", AT_INT_ARRAY);
	DMEA_SetIntArray(ints, vec, 3);
	
	vec4 = DME_add_attribute(ele, "THIRD", AT_VECTOR4);
	DMEA_SetVec4(vec4, v4);

	DMEAttribute *vmtx = DME_add_attribute(ele, "FOURTH", AT_VMATRIX_ARRAY);
	DMEA_SetMtxArray(vmtx, vm, 1000);
	DMEA_GetMtxArray(vmtx, &back);

	DMEAttribute *str = DME_add_attribute(ele, "string-attr", AT_STRING);
	DMEA_SetString(str, s1);

	DMEAttribute *stringarray = DME_add_attribute(ele, "string array", AT_STRING_ARRAY);
	DMEA_SetStringArray(stringarray, sar, 3);

	MEM_printblocks(stdout);

	printf("f: %f\n", DMEA_GetFloat(attr));

	DMEA_GetIntArray(ints, (int **) &vi);

	printf("ia: %d %d %d\n", vi[0], vi[1], vi[2]);

	DME_remove_attribute(ele, attr);

	DMEA_GetString(str, &s2);

	sar_back = DMEA_GetStringArray(stringarray, sar_back);
	
	DM_remove_element(&dm, ele);

	for (int i = 0; i < 3; ++i) {
		printf("%s\n", sar_back[i]);
	}

	printf("%s\n", s2);

	_getch();

	DM_Release(&dm);

	MEM_free(vi);

	MEM_printblocks(stdout);

	_getch();

#endif


#ifdef TEST_MEM
	int *a = MEM_calloc(100, "ints");
	float *t = MEM_malloc(3123, "floats");

	MEM_printblocks(stdout);

	_getch();

	MEM_free(a);

	MEM_printblocks(stdout);	

	MEM_free(t);

	MEM_printblocks(stdout);

	a = MEM_malloc(123, "FIRST");
	a = MEM_malloc(11, "SECOND");
	t = MEM_malloc(3, "THIRD");

	MEM_printblocks(stdout);

	MEM_free(a);

	MEM_printblocks(stdout);

	_getch();
#endif 


	return 0;
}