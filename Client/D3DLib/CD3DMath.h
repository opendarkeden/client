//----------------------------------------------------------------------
// CD3DMath.h
//----------------------------------------------------------------------

#ifndef	__CD3DMATH_H__
#define	__CD3DMATH_H__

#ifndef	D3D_OVERLOADS
#define	D3D_OVERLOADS
#endif

#ifdef PLATFORM_WINDOWS
	#include <D3D.h>
#else
	// For non-Windows platforms, provide basic type definitions
	#include <math.h>

	// Basic types
	typedef float FLOAT;
	typedef float D3DVALUE;
	typedef int BOOL;
	// DWORD is already defined in Platform.h

	// Basic D3D types
	typedef struct _D3DVECTOR {
		D3DVALUE x; D3DVALUE y; D3DVALUE z;

		// Constructors
		_D3DVECTOR() : x(0), y(0), z(0) {}
		_D3DVECTOR(D3DVALUE _x, D3DVALUE _y, D3DVALUE _z) : x(_x), y(_y), z(_z) {}

		// Arithmetic operators (when D3D_OVERLOADS is defined)
		#ifdef D3D_OVERLOADS
		// Addition
		_D3DVECTOR operator+(const _D3DVECTOR& v) const {
			return _D3DVECTOR(x + v.x, y + v.y, z + v.z);
		}
		// Subtraction
		_D3DVECTOR operator-(const _D3DVECTOR& v) const {
			return _D3DVECTOR(x - v.x, y - v.y, z - v.z);
		}
		// Scalar multiplication
		_D3DVECTOR operator*(D3DVALUE s) const {
			return _D3DVECTOR(x * s, y * s, z * s);
		}
		// Scalar division
		_D3DVECTOR operator/(D3DVALUE s) const {
			return _D3DVECTOR(x / s, y / s, z / s);
		}
		// Dot product
		D3DVALUE operator*(const _D3DVECTOR& v) const {
			return x * v.x + y * v.y + z * v.z;
		}
		#endif
	} D3DVECTOR, *LPD3DVECTOR;

	// D3D Matrix (4x4)
	typedef struct _D3DMATRIX {
		D3DVALUE _11, _12, _13, _14;
		D3DVALUE _21, _22, _23, _24;
		D3DVALUE _31, _32, _33, _34;
		D3DVALUE _41, _42, _43, _44;
	} D3DMATRIX, *LPD3DMATRIX;

	// D3D Vertex types
	typedef struct _D3DVERTEX {
		D3DVALUE x, y, z;           // Position coordinates (NOT D3DVECTOR!)
		D3DVALUE nx, ny, nz;        // Normal vector coordinates (NOT D3DVECTOR!)
		D3DVALUE tu, tv;            // Texture coordinates

		// Default constructor
		_D3DVERTEX() : x(0), y(0), z(0), nx(0), ny(0), nz(0), tu(0), tv(0) {}
	} D3DVERTEX, *LPD3DVERTEX;

	typedef struct _D3DTLVERTEX {
		// Direct3D transformed/lit vertex
		// Standard naming: sx/sy/sz (screen coordinates)
		// Also support dvSX/dvSY/dvSZ naming for compatibility
		union {
			struct { D3DVALUE sx, sy, sz; };
			struct { D3DVALUE dvSX, dvSY, dvSZ; };
		};
		D3DVALUE rhw;
		D3DVALUE color, specular;
		D3DVALUE tu, tv;

		// Default constructor
		_D3DTLVERTEX() : sx(0), sy(0), sz(0), rhw(0), color(0), specular(0), tu(0), tv(0) {}
	} D3DTLVERTEX, *LPD3DTLVERTEX;
#endif

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

		//---------------------------------------------------------------
		//
		//                     Vector Helpers
		//
		//---------------------------------------------------------------
		// Cross product of two vectors
		static inline D3DVECTOR CrossProduct( const D3DVECTOR& a, const D3DVECTOR& b ) {
			return D3DVECTOR(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		// Normalize vector (returns normalized copy, doesn't modify input)
		static inline D3DVECTOR Normalize( const D3DVECTOR& v ) {
			D3DVALUE len = (D3DVALUE)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
			if (len > g_EPSILON) {
				return D3DVECTOR(v.x / len, v.y / len, v.z / len);
			}
			return D3DVECTOR(0.0f, 0.0f, 0.0f);
		}
};

#endif




