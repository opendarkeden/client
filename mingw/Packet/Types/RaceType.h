#ifndef __RACETYPE_H__
#define __RACETYPE_H__
//////////////////////////////////////////////////////////////////////////////
// Race
//////////////////////////////////////////////////////////////////////////////
enum Race
{
	RACE_SLAYER = 0,
	RACE_VAMPIRE,
	RACE_OUSTERS,

	RACE_MAX
};

#define	FLAG_RACE_SLAYER	1
#define	FLAG_RACE_VAMPIRE	2
#define	FLAG_RACE_OUSTERS	4

typedef unsigned char Race_t;
const unsigned int szRace = sizeof(Race_t);

#endif