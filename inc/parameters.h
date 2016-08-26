#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

// This file will hold all the parameters needed for the program.
// Though they're public (temporarily), one SHOULD NOT try to change them.

namespace Parameters
{
	extern int PATCH_SIZE_THRESH;	//Minimal number of points inside a patch
	extern int N_BEST_PAIRS;		//Number of complementary pairs we'll store for each patch in target
	extern double G_THRESH;			//Geodesic threshold used for grouping
};

#endif