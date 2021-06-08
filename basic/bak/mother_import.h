#ifndef __MOTHER_IMPORT_H__
#define __MOTHER_IMPORT_H__

#include "mother.h"
#include "DLL.h"

DllImport double Proportion(double target, double target_max, double base);
DllImport double Percentage(double target, double target_max);

#endif