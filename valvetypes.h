#ifndef _VALVETYPES_H_
#define _VALVETYPES_H_

/* these are data types used by the Source Engine, as they're in memory during runtime */
/* these are all simple types and are really included for better readability of getters and setters */
/* 100% safe to cast to normal float/int arrays! */

typedef struct VColor {
	int r, g, b, a;
} VColor;

typedef struct VMatrix {
	float m[4][4];
} VMatrix;

typedef struct VVec3 {
	float x, y, z;
} VVec3;

typedef struct VVec2 {
	float x, y;
} VVec2;

typedef struct VVec4 {
	float x, y, z, w;
} VVec4;

typedef struct VQuat {
	float x, y, z, w;
} VQuat;

typedef struct VTimeValue {
	float t;
} VTimeValue;

typedef struct VQAngles {
	float x, y, z;
} VQAngles;

#endif