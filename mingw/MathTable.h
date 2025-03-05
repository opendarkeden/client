//-----------------------------------------------------------------------------
// MathTable.h
//-----------------------------------------------------------------------------
// 개조 버전
//-----------------------------------------------------------------------------
//
//   -1 <= (FSin(angle) >> 16) <= 1   이다.
//
// 즉, FSin( angle )해서 얻은 값을 >> 16 해야 제대로된 Sin값이다.
//
// 값은 아래와 같다.
// (FSin( 0 ) >> 16) == 0
// (FSin( 512 ) >> 16) == 1
// (FSin( 1024 ) >> 16) == 0
// (FSin( 1536 ) >> 16) == -1
// (FSin( 2048 ) >> 16) == 0
//-----------------------------------------------------------------------------

#ifndef __MATHTABLE_H__
#define __MATHTABLE_H__


namespace MathTable {

	//--------------------------------------------------------------
	// const
	//--------------------------------------------------------------
	const int MAX_ANGLE = 2048;
	const int MAX_ANGLE2 = MAX_ANGLE*2;
	const int MAX_ANGLE_1 = MAX_ANGLE-1;
	const int MAX_ANGLE2_1 = MAX_ANGLE2-1;
	const int MAX_ANGLE_HALF = MAX_ANGLE/2;
	const int FPI		= MAX_ANGLE_HALF;
	const int FPI2		= MAX_ANGLE;

	const int ANGLE_0	= 0;
	const int ANGLE_30	= FPI/6;
	const int ANGLE_45	= FPI/4;
	const int ANGLE_60	= FPI/3;
	const int ANGLE_90	= FPI/2;
	const int ANGLE_120	= ANGLE_90 + ANGLE_30;
	const int ANGLE_135 = ANGLE_90 + ANGLE_45;
	const int ANGLE_150	= ANGLE_90 + ANGLE_60;
	const int ANGLE_180 = FPI;
	const int ANGLE_210	= ANGLE_180 + ANGLE_30;
	const int ANGLE_225 = ANGLE_180+ANGLE_45;
	const int ANGLE_240	= ANGLE_180 + ANGLE_60;
	const int ANGLE_270 = FPI*3/2;
	const int ANGLE_300	= ANGLE_270 + ANGLE_30;
	const int ANGLE_315 = ANGLE_270+ANGLE_45;
	const int ANGLE_330	= ANGLE_270 + ANGLE_60;
	const int ANGLE_360 = FPI2;

	//--------------------------------------------------------------
	// init
	//--------------------------------------------------------------
	extern void FCreateSines();

	//--------------------------------------------------------------
	// tables
	//--------------------------------------------------------------
	extern int FSinTab[MAX_ANGLE];
	extern int FCosTab[MAX_ANGLE];
	extern int FArcTanTab[MAX_ANGLE+1];	// 실제로 angle 개수와는 무관하다.


	//--------------------------------------------------------------
	// get functions
	//--------------------------------------------------------------
	extern int FSin(int angle);
	extern int FCos(int angle);
	extern int FArcTan(int angle);

	//--------------------------------------------------------------
	// functions
	//--------------------------------------------------------------
	extern int GetAngle360(int angle);		// 2FPI --> 360
	extern int GetAngleToTarget(int x, int y, int targetX, int targetY);
	extern int GetAngleDir(int currentAngle, int targetAngle);
	extern int ClipAngle(int angle);
};


#endif
