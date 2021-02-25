//----------------------------------------------------------------------
// CD3DMath.h
//----------------------------------------------------------------------

#ifndef	__CD3DMATH_H__
#define	__CD3DMATH_H__

#ifndef	D3D_OVERLOADS
#define	D3D_OVERLOADS
#endif

#include <D3D.h>

//-----------------------------------------------------------------------------
// Useful Math constants
//-----------------------------------------------------------------------------
const float g_PI       =  3.14159265358979323846f; // Pi
const float g_2_PI     =  6.28318530717958623200f; // 2 * Pi
const float g_PI_DIV_2 =  1.57079632679489655800f; // Pi / 2
const float g_PI_DIV_4 =  0.78539816339744827900f; // Pi / 4
const float g_INV_PI   =  0.31830988618379069122f; // 1 / Pi
const float g_DEGTORAD =  0.01745329251994329547f; // Degrees to Radians
const float g_RADTODEG = 57.29577951308232286465f; // Radians to Degrees
const float g_HUGE     =  1.0e+38f;                // Huge number for float
const float g_EPSILON  =  1.0e-5f;                 // Tolerance for floats

//-----------------------------------------------------------------------------
// Fuzzy compares (within tolerance)
//-----------------------------------------------------------------------------
inline BOOL D3DMath_IsZero( FLOAT a, FLOAT fTol = g_EPSILON )
{ return ( a <= 0.0f ) ? ( a >= -fTol ) : ( a <= fTol ); }


class CD3DMath {
	public :
		//---------------------------------------------------------------
		//
		//                         Matrix 
		//
		//---------------------------------------------------------------
		// Identity
		static void			GetIdentityMatrix( D3DMATRIX &mat );		

		// Translate
		static void			GetTranslateMatrix( D3DMATRIX& mat, const D3DVECTOR &Vect );
		static void			GetTranslateMatrix( D3DMATRIX& mat, float sx, float sy, float sz );

		// Scale
		static void			GetScaleMatrix( D3DMATRIX& mat, float sx, float sy, float sz );
		static void			GetScaleMatrix( D3DMATRIX& mat, const D3DVECTOR& v );		

		// Rotate
		static void			GetRotateXMatrix( D3DMATRIX& mat, float fRads );
		static void			GetRotateYMatrix( D3DMATRIX& mat, float fRads );
		static void			GetRotateZMatrix( D3DMATRIX& mat, float fRads );				
		static void			GetRotateMatrix( D3DMATRIX& mat, const D3DVECTOR& vDir, float fRads );


		//---------------------------------------------------------------
		//
		//                     Matrix Calculation
		//
		//---------------------------------------------------------------		
		// Utility
		static void			MultiplyMatrix( D3DMATRIX& q, const D3DMATRIX& a, const D3DMATRIX& b );
		static bool			InvertMatrix( D3DMATRIX& q, const D3DMATRIX& a );

		// Matrix * Vector
		static bool			MultiplyMatrixToVertex(const D3DMATRIX &mat, D3DTLVERTEX &Vert);
		static bool			MultiplyMatrixToVertex(const D3DMATRIX &mat, D3DVERTEX &Vert);
		static bool			MultiplyMatrixToVector(const D3DMATRIX &mat, D3DVECTOR &Vector);

		// Matrix *  Translate Matrix
		static void			MultiplyTranslateMatrix( D3DMATRIX& mat, const D3DVECTOR &Vect );
		static void			MultiplyTranslateMatrix( D3DMATRIX& mat, float sx, float sy, float sz );

		// Matrix *  Scale Matrix
		static void			MultiplyScaleMatrix( D3DMATRIX& mat, float sx, float sy, float sz );
		static void			MultiplyScaleMatrix( D3DMATRIX& mat, const D3DVECTOR& v );		

		// Matrix *  Rotate Matrix
		static void			MultiplyRotateXMatrix( D3DMATRIX& mat, float fRads );
		static void			MultiplyRotateYMatrix( D3DMATRIX& mat, float fRads );
		static void			MultiplyRotateZMatrix( D3DMATRIX& mat, float fRads );		
		static void			MultiplyRotateVectorMatrix( D3DMATRIX& mat, const D3DVECTOR& vDir, float fRads );
		
		
		//---------------------------------------------------------------
		//
		//               VECTOR / VERTEX / TLVERTEX
		//
		//---------------------------------------------------------------
		// VECTOR
		static void			NormalizeVector( D3DVECTOR &Vector );
		static void			MoveVector( D3DVECTOR &Vector, float move_x, float move_y, float move_z );
		static void			RotateVector( D3DVECTOR &Vector, float fRadx, float fRady, float fRadz );
		static void			RotateVectorX( D3DVECTOR &Vector, float fRadx );
		static void			RotateVectorY( D3DVECTOR &Vector, float fRady );
		static void			RotateVectorZ( D3DVECTOR &Vector, float fRadz );
		static void			ScaleVector( D3DVECTOR &Vector, float scale_x, float scale_y, float scale_z );

		// VERTEX
		static void			MoveVertex( D3DVERTEX &Vertex, float move_x, float move_y, float move_z );
		static void			RotateVertex( D3DVERTEX &Vertex, float fRadx, float fRady, float fRadz );
		static void			RotateVertexX( D3DVERTEX &Vertex, float fRadx );
		static void			RotateVertexY( D3DVERTEX &Vertex, float fRady );
		static void			RotateVertexZ( D3DVERTEX &Vertex, float fRadz );
		static void			ScaleVertex( D3DVERTEX &Vertex, float scale_x, float scale_y, float scale_z );
	
		// TLVERTEX
		static void			MoveVertex( D3DTLVERTEX &Vertex, float move_x, float move_y, float move_z );
		static void			RotateVertex( D3DTLVERTEX &Vertex, float fRadx, float fRady, float fRadz );
		static void			RotateVertexX( D3DTLVERTEX &Vertex, float fRadx );
		static void			RotateVertexY( D3DTLVERTEX &Vertex, float fRady );
		static void			RotateVertexZ( D3DTLVERTEX &Vertex, float fRadz );
		static void			ScaleVertex( D3DTLVERTEX &Vertex, float scale_x, float scale_y, float scale_z );
		

		//---------------------------------------------------------------
		//
		//                         Quaternion
		//
		//---------------------------------------------------------------
		static void		QuaternionFromRotation( float& x, float& y, float& z, float& w, D3DVECTOR& v, float fTheta );
		static void		RotationFromQuaternion( D3DVECTOR& v, float& fTheta, float x, float y, float z, float w ); 
		static void		QuaternionFromAngles( float& x, float& y, float& z, float& w, float fYaw, float fPitch, float fRoll );
		static void		MatrixFromQuaternion( D3DMATRIX& mat, float x, float y, float z, float w );
		static void		QuaternionFromMatrix( float& x, float& y, float& z, float& w, D3DMATRIX& mat );
		static void		QuaternionMultiply( float& Qx, float& Qy, float& Qz, float& Qw,
											float Ax, float Ay, float Az, float Aw,
											float Bx, float By, float Bz, float Bw );
		static void		QuaternionSlerp( float& Qx, float& Qy, float& Qz, float& Qw,
										float Ax, float Ay, float Az, float Aw,
										float Bx, float By, float Bz, float Bw,
										float fAlpha );


		//---------------------------------------------------------------
		// 
		//                       Plane
		//
		//---------------------------------------------------------------
		static float*	GetPlane( float* plane, D3DVECTOR& a, D3DVECTOR& b, D3DVECTOR& c );
};

#endif




