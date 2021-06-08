#include "client_PCH.h"
#include "mother.h"

//-----------------------------------------------------------------------------
// Proportion
//
// (target <= target_max)
// base는 표현하려는 기준이다.
//
// ex) target = 50, target_max = 100, base = 51
//		 => 100에 대한 50이 base인 51에서 얼마인가를 구한다.
//
// 이것은 백분율을 구하는 식과 같다. 언제나 100이 아닐 경우 이것을 사용하면 된다.
//
// 반환되는 값 x는 (0 <= x <= base)이다.
//-----------------------------------------------------------------------------
double Proportion(double target, double target_max, double base)
{
	if (target > target_max)
		target = target_max;
	if (target_max == 0)
		return 0;

	return (((double)target/target_max)*base);
}

//-----------------------------------------------------------------------------
// Percentage
//
// 백분율을 구한다.
//-----------------------------------------------------------------------------
double Percentage(double target, double target_max)
{
	return Proportion(target, target_max, PERCENTAGE);
}