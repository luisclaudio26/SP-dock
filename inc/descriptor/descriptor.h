#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#include "../graph/convexity.h"

typedef struct {
	double curv;
	Convexity type;
} Descriptor;

#endif