/*-----------------------------------------------------------------------------

	mother.h

	Mother - My mathematics solution.

	Mother master header file.

	2000.8.2 KJTINC

-----------------------------------------------------------------------------*/

#ifndef __MOTHER_H__
#define __MOTHER_H__

#include "typedef.h"
#include <math.h>

#define PERCENTAGE						100

double Proportion(double target, double target_max, double base);
double Percentage(double target, double target_max);

#endif