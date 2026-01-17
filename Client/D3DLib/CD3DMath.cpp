//------------------------------------------------------------------------------------
// CD3DMath.h
//------------------------------------------------------------------------------------

#include "CD3DMath.h"
#include <math.h>
#include <string.h>

// Platform-specific memory operations
#ifndef PLATFORM_WINDOWS
	// ZeroMemory is Windows-specific, use memset on other platforms
	#define ZeroMemory(ptr, size) memset((ptr), 0, (size))
#endif

//------------------------------------------------------------------------------------
// Static members
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Get Identity Matrix
//------------------------------------------------------------------------------------
// I Matrix를 구한다. 
//------------------------------------------------------------------------------------
void 
CD3DMath::GetIdentityMatrix(D3DMATRIX &mat)
{
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;

	mat._12 = mat._13 = mat._14 = 0.0f;
	mat._21 = mat._23 = mat._24 = 0.0f;
	mat._31 = mat._32 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;
}

//------------------------------------------------------------------------------------
// Get Translate Matrix
//------------------------------------------------------------------------------------
// Vector(x,y,z)만큼 이동하는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void
CD3DMath::GetTranslateMatrix(D3DMATRIX& mat, const D3DVECTOR &Vect)
{
	// identity Matrix
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = 0.0f;
	mat._21 = mat._23 = mat._24 = 0.0f;
	mat._31 = mat._32 = mat._34 = 0.0f;	

	// 이동
	mat._41 = Vect.x;
	mat._42 = Vect.y;
	mat._43 = Vect.z;
}

//------------------------------------------------------------------------------------
// Get Translate Matrix
//------------------------------------------------------------------------------------
// Vector(x,y,z)만큼 이동하는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetTranslateMatrix( D3DMATRIX& mat, float sx, float sy, float sz )
{
	// identity Matrix
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = 0.0f;
	mat._21 = mat._23 = mat._24 = 0.0f;
	mat._31 = mat._32 = mat._34 = 0.0f;	

	// 이동
	mat._41 = sx;
	mat._42 = sy;
	mat._43 = sz;
}

//------------------------------------------------------------------------------------
// Get Scale Matrix
//------------------------------------------------------------------------------------
// 크기를 바꾸는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetScaleMatrix( D3DMATRIX& mat, float sx, float sy, float sz )
{
	// Identity Matrix
	mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = 0.0f;
	mat._21 = mat._23 = mat._24 = 0.0f;
	mat._31 = mat._32 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;

	// Scale
	mat._11 = sx; 
	mat._22 = sy; 
	mat._33 = sz;
}

//------------------------------------------------------------------------------------
// Get Scale Matrix
//------------------------------------------------------------------------------------
// 크기를 바꾸는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetScaleMatrix( D3DMATRIX& mat, const D3DVECTOR& v )
{
	// Identity Matrix
	mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = 0.0f;
	mat._21 = mat._23 = mat._24 = 0.0f;
	mat._31 = mat._32 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;

	// Scale
	mat._11 = v.x; 
	mat._22 = v.y; 
	mat._33 = v.z;
}

//------------------------------------------------------------------------------------
// Get Rotate Matrix
//------------------------------------------------------------------------------------
// X축 중심으로 회전하는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetRotateXMatrix( D3DMATRIX& mat, float fRads )
{
	// Identity Matrix
	mat._11 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = 0.0f;
	mat._21 = mat._24 = 0.0f;
	mat._31 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;

	// X축 회전
	float cf = cosf( fRads );
	float sf = sinf( fRads );

    mat._22 =  cf;
    mat._23 =  sf;
    mat._32 = -sf;
    mat._33 =  cf;
}

//------------------------------------------------------------------------------------
// Get RotateX Matrix
//------------------------------------------------------------------------------------
// Y축 중심으로 회전하는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetRotateYMatrix( D3DMATRIX& mat, float fRads )
{
	// Identity Matrix
	mat._22 = mat._44 = 1.0f;
	mat._12 = mat._14 = 0.0f;
	mat._21 = mat._23 = mat._24 = 0.0f;
	mat._32 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;

	// Y축 회전
	float cf = cosf( fRads );
	float sf = sinf( fRads );

    mat._11 =  cf;
    mat._13 = -sf;
    mat._31 =  sf;
    mat._33 =  cf;
}

//------------------------------------------------------------------------------------
// Get RotateY Matrix
//------------------------------------------------------------------------------------
// Z축 중심으로 회전하는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetRotateZMatrix( D3DMATRIX& mat, float fRads )
{
	// Identity Matrix
	mat._33 = mat._44 = 1.0f;
	mat._13 = mat._14 = 0.0f;
	mat._23 = mat._24 = 0.0f;
	mat._31 = mat._32 = mat._34 = 0.0f;
	mat._41 = mat._42 = mat._43 = 0.0f;

	// Z축 회전
	float cf = cosf( fRads );
	float sf = sinf( fRads );

    mat._11  =  cf;
    mat._12  =  sf;
    mat._21  = -sf;
    mat._22  =  cf;
}

//------------------------------------------------------------------------------------
// Get RotateZ Matrix
//------------------------------------------------------------------------------------
// vDir Vector를 중심으로 fRad만큼 회전하는 Matrix를 생성한다.
//------------------------------------------------------------------------------------
void			
CD3DMath::GetRotateMatrix( D3DMATRIX& mat, const D3DVECTOR& vDir, float fRads )
{
	// Normalize
	D3DVECTOR v = vDir;	
	NormalizeVector( v );

	float	fCos = cosf( fRads );
    float	fSin = sinf( fRads );    
	
	float	OneMinusfCos = 1.0f;
	OneMinusfCos -= fCos;
	
	float	XSin, YSin, ZSin;
	XSin = v.x;
	XSin *= fSin;

	YSin = v.y;
	YSin *= fSin;	

	ZSin = v.z;
	ZSin *= fSin;

	float	XYOneMinusfCos,	YZOneMinusfCos,	ZXOneMinusfCos;
	XYOneMinusfCos = v.x;
	XYOneMinusfCos *= v.y;
	XYOneMinusfCos *= OneMinusfCos;

	YZOneMinusfCos = v.y;
	YZOneMinusfCos *= v.z;
	YZOneMinusfCos *= OneMinusfCos;

	ZXOneMinusfCos = v.z;
	ZXOneMinusfCos *= v.x;
	ZXOneMinusfCos *= OneMinusfCos;		

    mat._11 = v.x;
	mat._11	*= v.x;
	mat._11 *= OneMinusfCos;
	mat._11 += fCos;

    mat._12 = XYOneMinusfCos;
	mat._12 -= ZSin;

    mat._13 = ZXOneMinusfCos;
	mat._13 += YSin;

    mat._21 = XYOneMinusfCos;
	mat._21 += ZSin;

    mat._22 = v.y;
	mat._22 *= v.y;
	mat._22 *= OneMinusfCos;
	mat._22 += fCos;

    mat._23 = YZOneMinusfCos;
	mat._23 -= XSin;

    mat._31 = ZXOneMinusfCos;
	mat._31 -= YSin;

    mat._32 = YZOneMinusfCos;
	mat._32 += XSin;

    mat._33 = v.z;
	mat._33 *= v.z;
	mat._33 *= OneMinusfCos;
	mat._33 += fCos;

    mat._14 = mat._24 = mat._34 = 0.0f;
    mat._41 = mat._42 = mat._43 = 0.0f;
    mat._44 = 1.0f;
}

//-------------------------------------------------------------------------------------------
// Multiply Matrix
//-------------------------------------------------------------------------------------------
// Desc: Does the matrix operation: [Q] = [A] * [B]. Note that the order of
//       this operation was changed from the previous version of the DXSDK.
//-------------------------------------------------------------------------------------------
void 
CD3DMath::MultiplyMatrix( D3DMATRIX& q, const D3DMATRIX& a, const D3DMATRIX& b )
{
    float* pA = (float*)&a;
    float* pB = (float*)&b;
    float  pM[16];

    ZeroMemory( pM, sizeof(D3DMATRIX) );
	
	register int i;
	register int j;
	register int k;

    for(i=0; i<4; i++ ) 
        for(j=0; j<4; j++ ) 
            for(k=0; k<4; k++ ) 
                pM[4*i+j] +=  pA[4*i+k] * pB[4*k+j];

    memcpy( &q, pM, sizeof(D3DMATRIX) );
}

//-------------------------------------------------------------------------------------------
// Invert Matrix
//-------------------------------------------------------------------------------------------
// Desc: Does the matrix operation: [Q] = inv[A]. Note: this function only
//       works for matrices with [0 0 0 1] for the 4th column.
//-------------------------------------------------------------------------------------------
bool 
CD3DMath::InvertMatrix( D3DMATRIX& q, const D3DMATRIX& a )
{
    if( fabs(a._44 - 1.0f) > .001f)
        return false;//E_INVALIDARG;
    if( fabs(a._14) > .001f || fabs(a._24) > .001f || fabs(a._34) > .001f )
        return false;//E_INVALIDARG;

    float fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                             a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                             a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0f;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0f;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0f;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0f;

    return true;//S_OK;
}

//-----------------------------------------------------------------------------
// Matrix *  Translate Matrix
//-----------------------------------------------------------------------------
// mat에 Vect만큼 이동하는 Matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyTranslateMatrix( D3DMATRIX& mat, const D3DVECTOR &Vect )
{
	D3DMATRIX matTemp;
	GetTranslateMatrix( matTemp, Vect );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * matTemp
}

//-----------------------------------------------------------------------------
// Matrix * Translate Matrix
//-----------------------------------------------------------------------------
// mat에 (sx,sy,sz)만큼 이동하는 Matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyTranslateMatrix( D3DMATRIX& mat, float sx, float sy, float sz )
{
	D3DMATRIX matTemp;
	GetTranslateMatrix( matTemp, sx, sy, sz );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * matTemp
}

//-----------------------------------------------------------------------------
// Matrix *  Scale Matrix
//-----------------------------------------------------------------------------
// mat에 각 방향으로 (sx,sy,sz)만큼 크기를 바꾸는 Matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyScaleMatrix( D3DMATRIX& mat, float sx, float sy, float sz )
{
	D3DMATRIX matTemp;
	GetScaleMatrix( matTemp, sx, sy, sz );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * matTemp
}

//-----------------------------------------------------------------------------
// Matrix *  Scale Matrix
//-----------------------------------------------------------------------------
// mat에 각 방향으로 v vector만큼 크기를 바꾸는 Matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyScaleMatrix( D3DMATRIX& mat, const D3DVECTOR& v )
{
	D3DMATRIX matTemp;
	GetScaleMatrix( matTemp, v );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * matTemp
}

//-----------------------------------------------------------------------------
// Matrix *  RotateX Matrix
//-----------------------------------------------------------------------------
// mat에 Matrix에 X축으로 fRads만큼 회전하는 matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyRotateXMatrix( D3DMATRIX& mat, float fRads )
{
	D3DMATRIX matTemp;
	GetRotateXMatrix( matTemp, fRads );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * mat
}

//-----------------------------------------------------------------------------
// Matrix *  RotateY Matrix
//-----------------------------------------------------------------------------
// mat에 Matrix에 Y축으로 fRads만큼 회전하는 matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyRotateYMatrix( D3DMATRIX& mat, float fRads )
{
	D3DMATRIX matTemp;
	GetRotateYMatrix( matTemp, fRads );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * mat
}

//-----------------------------------------------------------------------------
// Matrix *  RotateZ Matrix
//-----------------------------------------------------------------------------
// mat에 Matrix에 Z축으로 fRads만큼 회전하는 matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyRotateZMatrix( D3DMATRIX& mat, float fRads )
{
	D3DMATRIX matTemp;
	GetRotateZMatrix( matTemp, fRads );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * mat
}

//-----------------------------------------------------------------------------
// Matrix *  RotateZ Matrix
//-----------------------------------------------------------------------------
// mat에 Matrix에 Vector를 중심으로 fRads만큼 회전하는 matrix를 곱한다.
//-----------------------------------------------------------------------------
void			
CD3DMath::MultiplyRotateVectorMatrix( D3DMATRIX& mat, const D3DVECTOR& vDir, float fRads )
{
	D3DMATRIX matTemp;
	GetRotateMatrix( matTemp, vDir, fRads );
	MultiplyMatrix(mat, mat, matTemp);	// mat = mat * mat
}
	

//------------------------------------------------------------------------------------
// Matrix * Vertex
//------------------------------------------------------------------------------------
// Vertex에 Matrix를 곱한다. 
// 4행은 이동에 관한것이므로 더한다.
//------------------------------------------------------------------------------------
bool 
CD3DMath::MultiplyMatrixToVertex(const D3DMATRIX &mat, D3DTLVERTEX &Vert)
{
	float temp_x, temp_y, temp_z, temp_w;

	temp_x = mat._11*Vert.dvSX + mat._21*Vert.dvSY + mat._31*Vert.dvSZ + mat._41;
	temp_y = mat._12*Vert.dvSX + mat._22*Vert.dvSY + mat._32*Vert.dvSZ + mat._42;
	temp_z = mat._13*Vert.dvSX + mat._23*Vert.dvSY + mat._33*Vert.dvSZ + mat._43;
	temp_w = mat._14*Vert.dvSX + mat._24*Vert.dvSY + mat._34*Vert.dvSZ + mat._44;

	if(fabs(temp_w)<0.00001) return false;
	
	Vert.dvSX = temp_x/temp_w;
	Vert.dvSY = temp_y/temp_w;
	Vert.dvSZ = temp_z/temp_w;

	return true;
}



//------------------------------------------------------------------------------------
// MoveVertex
//------------------------------------------------------------------------------------
// Vertex를 (x,y,z)만큼 이동시킨다.
//------------------------------------------------------------------------------------
void 
CD3DMath::MoveVertex( D3DVERTEX &Vertex, float move_x, float move_y, float move_z )
{
	Vertex.x += move_x;
	Vertex.y += move_y;
	Vertex.z += move_z;
}

//------------------------------------------------------------------------------------
// Rotate Vertex(x,y,z)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertex( D3DVERTEX &Vertex, float fRadx, float fRady, float fRadz )
{
	float x,y,z;
	float fc, fs;

	if( fRadx != 0 )
	{
		fc = cosf( fRadx );
		fs = sinf( fRadx );

		// homogeneous coordinates 
		y = Vertex.y*fc - Vertex.z*fs;
	    z = Vertex.y*fs + Vertex.z*fc;
		Vertex.y = y;
		Vertex.z = z;

		// normal
		y = Vertex.ny*fc - Vertex.nz*fs;
	    z = Vertex.ny*fs + Vertex.nz*fc;
		Vertex.ny = y;
		Vertex.nz = z;
	}

	if( fRady != 0 )
	{
		fc = cosf( fRady );
		fs = sinf( fRady );

		// homogeneous coordinates 
		x = Vertex.x*fc + Vertex.z*fs;
	    z = -Vertex.x*fs + Vertex.z*fc;
		Vertex.x = x;
		Vertex.z = z;

		// normal
		x = Vertex.nx*fc + Vertex.nz*fs;
	    z = -Vertex.nx*fs + Vertex.nz*fc;
		Vertex.nx = x;
		Vertex.nz = z;
	}

	if( fRadz != 0 )
	{
		fc = cosf( fRadz );
		fs = sinf( fRadz );

		// homogeneous coordinates 
		x = Vertex.x*fc - Vertex.y*fs;
		y = Vertex.x*fs + Vertex.y*fc;
		Vertex.x = x;
		Vertex.y = y;

		// normal
		x = Vertex.nx*fc - Vertex.ny*fs;
		y = Vertex.nx*fs + Vertex.ny*fc;
		Vertex.nx = x;
		Vertex.ny = y;
	}

}

//------------------------------------------------------------------------------------
// Rotate VertexX(x)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertexX( D3DVERTEX &Vertex, float fRadx)
{
	float y,z;
	float fc, fs;

	if( fRadx != 0 )
	{
		fc = cosf( fRadx );
		fs = sinf( fRadx );

		// homogeneous coordinates 
		y = Vertex.y*fc - Vertex.z*fs;
	    z = Vertex.y*fs + Vertex.z*fc;
		Vertex.y = y;
		Vertex.z = z;

		// normal
		y = Vertex.ny*fc - Vertex.nz*fs;
	    z = Vertex.ny*fs + Vertex.nz*fc;
		Vertex.ny = y;
		Vertex.nz = z;
	}
}

//------------------------------------------------------------------------------------
// Rotate VertexY(y)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertexY( D3DVERTEX &Vertex, float fRady)
{
	float x,z;
	float fc, fs;

	if( fRady != 0 )
	{
		fc = cosf( fRady );
		fs = sinf( fRady );

		// homogeneous coordinates 
		x = Vertex.x*fc + Vertex.z*fs;
	    z = -Vertex.x*fs + Vertex.z*fc;
		Vertex.x = x;
		Vertex.z = z;

		// normal
		x = Vertex.nx*fc + Vertex.nz*fs;
	    z = -Vertex.nx*fs + Vertex.nz*fc;
		Vertex.nx = x;
		Vertex.nz = z;
	}
}

//------------------------------------------------------------------------------------
// Rotate Vertex(x,y,z)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertexZ( D3DVERTEX &Vertex, float fRadz )
{
	float x,y;
	float fc, fs;

	if( fRadz != 0 )
	{
		fc = cosf( fRadz );
		fs = sinf( fRadz );

		// homogeneous coordinates 
		x = Vertex.x*fc - Vertex.y*fs;
		y = Vertex.x*fs + Vertex.y*fc;
		Vertex.x = x;
		Vertex.y = y;

		// normal
		x = Vertex.nx*fc - Vertex.ny*fs;
		y = Vertex.nx*fs + Vertex.ny*fc;
		Vertex.nx = x;
		Vertex.ny = y;
	}
}

//------------------------------------------------------------------------------------
// Scale Vertex
//------------------------------------------------------------------------------------
void 
CD3DMath::ScaleVertex( D3DVERTEX &Vertex, float scale_x, float scale_y, float scale_z )
{
	if( Vertex.x == 0 )
	    Vertex.x = 0.001f;
	if( Vertex.y == 0 )
	    Vertex.y = 0.001f;
	if( Vertex.z == 0 )
        Vertex.z = 0.001f;

	if( scale_x == 0 )
        scale_x = 0.001f;
	if( scale_y == 0 )
        scale_y = 0.001f;
	if( scale_z == 0 )
        scale_z = 0.001f;

	Vertex.x *= scale_x;
	Vertex.y *= scale_y;
	Vertex.z *= scale_z;
}


//------------------------------------------------------------------------------------
// MoveVertex
//------------------------------------------------------------------------------------
// Vertex를 (x,y,z)만큼 이동시킨다.
//------------------------------------------------------------------------------------
void 
CD3DMath::MoveVertex( D3DTLVERTEX &Vertex, float move_x, float move_y, float move_z )
{
	Vertex.sx += move_x;
	Vertex.sy += move_y;
	Vertex.sz += move_z;
}

//------------------------------------------------------------------------------------
// Rotate Vertex(x,y,z)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertex( D3DTLVERTEX &Vertex, float fRadx, float fRady, float fRadz )
{
	float x,y,z;
	float fc, fs;

	if( fRadx != 0 )
	{
		fc = cosf( fRadx );
		fs = sinf( fRadx );

		// homogeneous coordinates 
		y = Vertex.sy*fc - Vertex.sz*fs;
	    z = Vertex.sy*fs + Vertex.sz*fc;
		Vertex.sy = y;
		Vertex.sz = z;
	}

	if( fRady != 0 )
	{
		fc = cosf( fRady );
		fs = sinf( fRady );

		// homogeneous coordinates 
		x = Vertex.sx*fc + Vertex.sz*fs;
	    z = -Vertex.sx*fs + Vertex.sz*fc;
		Vertex.sx = x;
		Vertex.sz = z;
	}

	if( fRadz != 0 )
	{
		fc = cosf( fRadz );
		fs = sinf( fRadz );

		// homogeneous coordinates 
		x = Vertex.sx*fc - Vertex.sy*fs;
		y = Vertex.sx*fs + Vertex.sy*fc;
		Vertex.sx = x;
		Vertex.sy = y;
	}
}

//------------------------------------------------------------------------------------
// Rotate VertexX(x)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertexX( D3DTLVERTEX &Vertex, float fRadx)
{
	float y,z;
	float fc, fs;

	if( fRadx != 0 )
	{
		fc = cosf( fRadx );
		fs = sinf( fRadx );

		// homogeneous coordinates 
		y = Vertex.sy*fc - Vertex.sz*fs;
	    z = Vertex.sy*fs + Vertex.sz*fc;
		Vertex.sy = y;
		Vertex.sz = z;
	}
}

//------------------------------------------------------------------------------------
// Rotate VertexY(y)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertexY( D3DTLVERTEX &Vertex, float fRady)
{
	float x,z;
	float fc, fs;

	if( fRady != 0 )
	{
		fc = cosf( fRady );
		fs = sinf( fRady );

		// homogeneous coordinates 
		x = Vertex.sx*fc + Vertex.sz*fs;
	    z = -Vertex.sx*fs + Vertex.sz*fc;
		Vertex.sx = x;
		Vertex.sz = z;
	}
}

//------------------------------------------------------------------------------------
// Rotate Vertex(x,y,z)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVertexZ( D3DTLVERTEX &Vertex, float fRadz )
{
	float x,y;
	float fc, fs;

	if( fRadz != 0 )
	{
		fc = cosf( fRadz );
		fs = sinf( fRadz );

		// homogeneous coordinates 
		x = Vertex.sx*fc - Vertex.sy*fs;
		y = Vertex.sx*fs + Vertex.sy*fc;
		Vertex.sx = x;
		Vertex.sy = y;
	}
}

//------------------------------------------------------------------------------------
// Scale Vertex
//------------------------------------------------------------------------------------
void 
CD3DMath::ScaleVertex( D3DTLVERTEX &Vertex, float scale_x, float scale_y, float scale_z )
{
	if( Vertex.sx == 0 )
	    Vertex.sx = 0.001f;
	if( Vertex.sy == 0 )
	    Vertex.sy = 0.001f;
	if( Vertex.sz == 0 )
        Vertex.sz = 0.001f;

	if( scale_x == 0 )
        scale_x = 0.001f;
	if( scale_y == 0 )
        scale_y = 0.001f;
	if( scale_z == 0 )
        scale_z = 0.001f;

	Vertex.sx *= scale_x;
	Vertex.sy *= scale_y;
	Vertex.sz *= scale_z;
}

//------------------------------------------------------------------------------------
// Matrix x Vertex
//------------------------------------------------------------------------------------
// Vertex에 Matrix를 곱한다. 
// 4행은 이동에 관한것이므로 더한다.
//------------------------------------------------------------------------------------
bool 
CD3DMath::MultiplyMatrixToVertex(const D3DMATRIX &mat, D3DVERTEX &Vert)
{
	float temp_x, temp_y, temp_z, temp_w;

	temp_x = mat._11*Vert.x + mat._21*Vert.y + mat._31*Vert.z + mat._41;
	temp_y = mat._12*Vert.x + mat._22*Vert.y + mat._32*Vert.z + mat._42;
	temp_z = mat._13*Vert.x + mat._23*Vert.y + mat._33*Vert.z + mat._43;
	temp_w = mat._14*Vert.x + mat._24*Vert.y + mat._34*Vert.z + mat._44;

	if(fabs(temp_w)<0.00001) return false;
	
	Vert.x = temp_x/temp_w;
	Vert.y = temp_y/temp_w;
	Vert.z = temp_z/temp_w;

	return true;
}

//------------------------------------------------------------------------------------
// Rotate Vector(x,y,z)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVector( D3DVECTOR &Vector, float fRadx, float fRady, float fRadz )
{
	float x,y,z;
	float fc,fs;

	if( fRadx != 0 )
	{
		fc = cosf( fRadx );
		fs = sinf( fRadx );

		y = Vector.y*fc - Vector.z*fs;
	    z = Vector.y*fs + Vector.z*fc;
		Vector.y = y;
		Vector.z = z;
	}

	if( fRady != 0 )
	{
		fc = cosf( fRady );
		fs = sinf( fRady );

		x = Vector.x*fc + Vector.z*fs;
	    z = -Vector.x*fs + Vector.z*fc;
		Vector.x = x;
		Vector.z = z;
	}

	if( fRadz != 0 )
	{
		fc = cosf( fRadz );
		fs = sinf( fRadz );

		x = Vector.x*fc - Vector.y*fs;
		y = Vector.x*fs + Vector.y*fc;
		Vector.x = x;
		Vector.y = y;
	}
}

//------------------------------------------------------------------------------------
// Rotate VectorX(x)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVectorX( D3DVECTOR &Vector, float fRadx)
{
	float y,z;
	float fc,fs;

	if( fRadx != 0 )
	{
		fc = cosf( fRadx );
		fs = sinf( fRadx );

		y = Vector.y*fc - Vector.z*fs;
	    z = Vector.y*fs + Vector.z*fc;
		Vector.y = y;
		Vector.z = z;
	}
}

//------------------------------------------------------------------------------------
// Rotate VectorY(y)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVectorY( D3DVECTOR &Vector, float fRady)
{
	float x,z;
	float fc,fs;

	if( fRady != 0 )
	{
		fc = cosf( fRady );
		fs = sinf( fRady );

		x = Vector.x*fc + Vector.z*fs;
	    z = -Vector.x*fs + Vector.z*fc;
		Vector.x = x;
		Vector.z = z;
	}
}

//------------------------------------------------------------------------------------
// Rotate Vector(x,y,z)
//------------------------------------------------------------------------------------
void 
CD3DMath::RotateVectorZ( D3DVECTOR &Vector, float fRadz )
{
	float x,y;
	float fc,fs;

	if( fRadz != 0 )
	{
		fc = cosf( fRadz );
		fs = sinf( fRadz );

		x = Vector.x*fc - Vector.y*fs;
		y = Vector.x*fs + Vector.y*fc;
		Vector.x = x;
		Vector.y = y;
	}
}

//------------------------------------------------------------------------------------
// Scale Vector
//------------------------------------------------------------------------------------
void 
CD3DMath::ScaleVector( D3DVECTOR &Vector, float scale_x, float scale_y, float scale_z )
{
	if( Vector.x == 0 )
	    Vector.x = 0.001f;
	if( Vector.y == 0 )
	    Vector.y = 0.001f;
	if( Vector.z == 0 )
        Vector.z = 0.001f;

	if( scale_x == 0 )
        scale_x = 0.001f;
	if( scale_y == 0 )
        scale_y = 0.001f;
	if( scale_z == 0 )
        scale_z = 0.001f;

	Vector.x *= scale_x;
	Vector.y *= scale_y;
	Vector.z *= scale_z;
}


//------------------------------------------------------------------------------------
// Matrix x Vector
//------------------------------------------------------------------------------------
// Vector에 Matrix를 곱한다. 
// 4행은 이동에 관한것이므로 더한다.
//------------------------------------------------------------------------------------
bool 
CD3DMath::MultiplyMatrixToVector(const D3DMATRIX &mat, D3DVECTOR &Vector)
{
	float temp_x, temp_y, temp_z, temp_w;

	temp_x = mat._11*Vector.x + mat._21*Vector.y + mat._31*Vector.z + mat._41;
	temp_y = mat._12*Vector.x + mat._22*Vector.y + mat._32*Vector.z + mat._42;
	temp_z = mat._13*Vector.x + mat._23*Vector.y + mat._33*Vector.z + mat._43;
	temp_w = mat._14*Vector.x + mat._24*Vector.y + mat._34*Vector.z + mat._44;

	if(fabs(temp_w)<0.00001) return false;
	
	Vector.x = temp_x/temp_w;
	Vector.y = temp_y/temp_w;
	Vector.z = temp_z/temp_w;

	return true;
}

//------------------------------------------------------------------------------------
// Normalize Vector
//------------------------------------------------------------------------------------
// 크기가 1인 Vector를 만든다.
//------------------------------------------------------------------------------------
void			
CD3DMath::NormalizeVector( D3DVECTOR &Vector )
{
	float len = (float)sqrt(Vector.x*Vector.x + Vector.y*Vector.y + Vector.z*Vector.z);
	
	if (len!=0)
	{
		Vector.x /= len;
		Vector.y /= len;
		Vector.z /= len;
	}	
}

//------------------------------------------------------------------------------------
// MoveVector
//------------------------------------------------------------------------------------
// Vector를 (x,y,z)만큼 이동시킨다.
//------------------------------------------------------------------------------------
void 
CD3DMath::MoveVector( D3DVECTOR &Vector, float move_x, float move_y, float move_z )
{
	Vector.x += move_x;
	Vector.y += move_y;
	Vector.z += move_z;
}


//-------------------------------------------------------------------------------------------
// Name: CD3DMath::QuaternionFromRotation()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-------------------------------------------------------------------------------------------
void CD3DMath::QuaternionFromRotation( float& x, float& y, float& z, float& w,
                                     D3DVECTOR& v, float fTheta )
{
    x = sinf( fTheta/2.0f ) * v.x;
    y = sinf( fTheta/2.0f ) * v.y;
    z = sinf( fTheta/2.0f ) * v.z;
    w = cosf( fTheta/2.0f );
}

//-------------------------------------------------------------------------------------------
// Name: CD3DMath::RotationFromQuaternion()
// Desc: Converts a normalized axis and angle to a unit quaternion.
//-------------------------------------------------------------------------------------------
void RotationFromQuaternion( D3DVECTOR& v, float& fTheta, float x, float y, float z, float w )                                     
{
    fTheta = acosf(w) * 2.0f;
    v.x    = x / sinf( fTheta/2.0f );
    v.y    = y / sinf( fTheta/2.0f );
    v.z    = z / sinf( fTheta/2.0f );
}


//-------------------------------------------------------------------------------------------
// Name: CD3DMath::QuaternionFromAngles()
// Desc: Converts euler angles to a unit quaternion.
//-------------------------------------------------------------------------------------------
void CD3DMath::QuaternionFromAngles( float& x, float& y, float& z, float& w,
                                   float fYaw, float fPitch, float fRoll )
                                        
{
    float fSinYaw   = sinf( fYaw/2.0f );
    float fSinPitch = sinf( fPitch/2.0f );
    float fSinRoll  = sinf( fRoll/2.0f );
    float fCosYaw   = cosf( fYaw/2.0f );
    float fCosPitch = cosf( fPitch/2.0f );
    float fCosRoll  = cosf( fRoll/2.0f );

    x = fSinRoll * fCosPitch * fCosYaw - fCosRoll * fSinPitch * fSinYaw;
    y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    w = fCosRoll * fCosPitch * fCosYaw + fSinRoll * fSinPitch * fSinYaw;
}




//-------------------------------------------------------------------------------------------
// Name: CD3DMath::MatrixFromQuaternion()
// Desc: Converts a unit quaternion into a rotation matrix.
//-------------------------------------------------------------------------------------------
void CD3DMath::MatrixFromQuaternion( D3DMATRIX& mat, float x, float y, float z,
                                   float w )
{
    float xx = x*x; float yy = y*y; float zz = z*z;
    float xy = x*y; float xz = x*z; float yz = y*z;
    float wx = w*x; float wy = w*y; float wz = w*z;
    
    mat._11 = 1 - 2 * ( yy + zz ); 
    mat._12 =     2 * ( xy - wz );
    mat._13 =     2 * ( xz + wy );

    mat._21 =     2 * ( xy + wz );
    mat._22 = 1 - 2 * ( xx + zz );
    mat._23 =     2 * ( yz - wx );

    mat._31 =     2 * ( xz - wy );
    mat._32 =     2 * ( yz + wx );
    mat._33 = 1 - 2 * ( xx + yy );

    mat._14 = mat._24 = mat._34 = 0.0f;
    mat._41 = mat._42 = mat._43 = 0.0f;
    mat._44 = 1.0f;
}


//-------------------------------------------------------------------------------------------
// Name: CD3DMath::QuaternionFromMatrix()
// Desc: Converts a rotation matrix into a unit quaternion.
//-------------------------------------------------------------------------------------------
void CD3DMath::QuaternionFromMatrix( float& x, float& y, float& z, float& w,
                                   D3DMATRIX& mat )
{
    if( mat._11 + mat._22 + mat._33 > 0.0f )
    {
        float s = sqrtf( mat._11 + mat._22 + mat._33 + mat._44 );

        x = (mat._23-mat._32) / (2*s);
        y = (mat._31-mat._13) / (2*s);
        z = (mat._12-mat._21) / (2*s);
        w = 0.5f * s;
    }
    else
    {


    }
    float xx = x*x; float yy = y*y; float zz = z*z;
    float xy = x*y; float xz = x*z; float yz = y*z;
    float wx = w*x; float wy = w*y; float wz = w*z;
    
    mat._11 = 1 - 2 * ( yy + zz ); 
    mat._12 =     2 * ( xy - wz );
    mat._13 =     2 * ( xz + wy );

    mat._21 =     2 * ( xy + wz );
    mat._22 = 1 - 2 * ( xx + zz );
    mat._23 =     2 * ( yz - wx );

    mat._31 =     2 * ( xz - wy );
    mat._32 =     2 * ( yz + wx );
    mat._33 = 1 - 2 * ( xx + yy );

    mat._14 = mat._24 = mat._34 = 0.0f;
    mat._41 = mat._42 = mat._43 = 0.0f;
    mat._44 = 1.0f;
}




//-------------------------------------------------------------------------------------------
// Name: CD3DMath::QuaternionMultiply()
// Desc: Mulitples two quaternions together as in {Q} = {A} * {B}.
//-------------------------------------------------------------------------------------------
void CD3DMath::QuaternionMultiply( float& Qx, float& Qy, float& Qz, float& Qw,
                                  float Ax, float Ay, float Az, float Aw,
                                  float Bx, float By, float Bz, float Bw )
{
    float Dx =  Ax*Bw + Ay*Bz - Az*By + Aw*Bx;
    float Dy = -Ax*Bz + Ay*Bw + Az*Bx + Aw*By;
    float Dz =  Ax*By - Ay*Bx + Az*Bw + Aw*Bz;
    float Dw = -Ax*Bx - Ay*By - Az*Bz + Aw*Bw;

    Qx = Dx; Qy = Dy; Qz = Dz; Qw = Dw;
}


//-------------------------------------------------------------------------------------------
// Name: CD3DMath::SlerpQuaternions()
// Desc: Compute a quaternion which is the spherical linear interpolation
//       between two other quaternions by dvFraction.
//-------------------------------------------------------------------------------------------
void CD3DMath::QuaternionSlerp( float& Qx, float& Qy, float& Qz, float& Qw,
                              float Ax, float Ay, float Az, float Aw,
                              float Bx, float By, float Bz, float Bw,
                              float fAlpha )
{
    // Compute dot product (equal to cosine of the angle between quaternions)
    float fCosTheta = Ax*Bx + Ay*By + Az*Bz + Aw*Bw;

    // Check angle to see if quaternions are in opposite hemispheres
    if( fCosTheta < 0.0f ) 
    {
        // If so, flip one of the quaterions
        fCosTheta = -fCosTheta;
        Bx = -Bx; By = -By; Bz = -Bz; Bw = -Bw;
    }

    // Set factors to do linear interpolation, as a special case where the
    // quaternions are close together.
    float fBeta = 1.0f - fAlpha;
    
    // If the quaternions aren't close, proceed with spherical interpolation
    if( 1.0f - fCosTheta > 0.001f ) 
    {   
        float fTheta = acosf( fCosTheta );
        
        fBeta  = sinf( fTheta*fBeta ) / sinf( fTheta);
        fAlpha = sinf( fTheta*fAlpha ) / sinf( fTheta);
    }

    // Do the interpolation
    Qx = fBeta*Ax + fAlpha*Bx;
    Qy = fBeta*Ay + fAlpha*By;
    Qz = fBeta*Az + fAlpha*Bz;
    Qw = fBeta*Aw + fAlpha*Bw;
}



//-----------------------------------------------------------------------------
// CreatePlane()
//-----------------------------------------------------------------------------
// 세 점 a,b,c 로 이루어지는 평면의 방정식(coefficient)을 얻는다.
//
// plane은 4개의 float(A,B,C,D)인데,
// Ax + By + Cz + D = 0 이다.
//-----------------------------------------------------------------------------
float* 
CD3DMath::GetPlane( float* plane, D3DVECTOR& a, D3DVECTOR& b, D3DVECTOR& c )
{
    D3DVECTOR n = Normalize( CrossProduct( c-b, c-a ) );
    plane[0] = n.x; plane[1] = n.y; plane[2] = n.z; 
    plane[3] = -( n.x*c.x + n.y*c.y + n.z*c.z );

    return plane;
}

