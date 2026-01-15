//----------------------------------------------------------------------
// CDirect3D.h
//----------------------------------------------------------------------
//
// Texture로 사용할 Surface에 추가되면 좋은(!?) flag
//
//    ddsd.ddsCaps.dwCaps2 |= DDSCAPS2_TEXTUREMANAGE;
//
// 그러나, Hardware가속이 되는 상태여야만 한다.
//----------------------------------------------------------------------
#ifndef	__CDIRECT3D_H__
#define	__CDIRECT3D_H__

#ifndef	D3D_OVERLOADS
#define	D3D_OVERLOADS
#endif

#ifdef PLATFORM_WINDOWS
	#include <D3D.h>
	#include <ddraw.h>

	//#include "d3dtextr.h"
	//#include "d3dutil.h"

	#include "CDirectDraw.h"




class CDirect3D : public CDirectDraw {
	public :
		CDirect3D() {}
		~CDirect3D() {}

		//--------------------------------------------------
		// 초기화
		//--------------------------------------------------
		static bool				Init();
		static void				Release();

		//--------------------------------------------------
		// Restore
		//--------------------------------------------------
		static BOOL				IsLost();
		static void				Restore();

		//--------------------------------------------------
		// Is Support HAL?
		//--------------------------------------------------
		static bool				CheckHAL();

		//--------------------------------------------------
		// Get
		//--------------------------------------------------
		static const LPDIRECT3DDEVICE7	GetDevice() { return m_pD3DD; }
		static bool						IsHAL()		{ return m_bHAL; }

		//--------------------------------------------------
		// Texture 생성 관련 정보
		//--------------------------------------------------
		static void		GetTextureSize(int& width, int& height);
		static DWORD	GetTextureWidthMax()	{ return m_TextureWidthMax; }
		static DWORD	GetTextureHeightMax()	{ return m_TextureHeightMax; }

		static bool		IsTexturePow2()			{ return m_bTexturePow2; }
		static bool		IsTextureSquareOnly()	{ return m_bTextureSquareOnly; }

		static const LPDDPIXELFORMAT	GetPixelFormat4444() { return &m_PixelFormat4444; }
		static const LPDDPIXELFORMAT	GetPixelFormat1555() { return &m_PixelFormat1555; }

		//--------------------------------------------------
		// PixelFormat
		//--------------------------------------------------
		static bool				FindBestPixelFormat(int bitsA, int bitsR, int bitsG, int bitsB,
													LPDDPIXELFORMAT pddpfBestMatch);

		// Device : Texture
		//static void				SetTexture(DWORD dwStage, const char* filename);

	public :
		// 3D Rendering Mode
		/*
		enum CD3DMODE
		{
			MODE_NULL = 0,
			MODE_HARDWARE,
			MODE_MMX,
			MODE_SOFTWARE
		};
		*/

	protected :
		static bool				Init(GUID* pDriverGUID, const GUID* pDeviceGUID);
		static bool				CheckDeviceForTexture();

	private :
		static WORD				GetNumberOfBits( DWORD dwMask );
		static HRESULT WINAPI	EnumZBufferCallback(DDPIXELFORMAT* DDP_Format ,VOID* DDP_Desired);
		static HRESULT WINAPI	EnumTextureCallback( DDPIXELFORMAT* pddpf, VOID* );


	protected :
		//CD3DMODE			m_Mode;		// 3D Rendering Mode

		static bool						m_bHAL;

		static LPDIRECT3D7				m_pD3D;
		static LPDIRECT3DDEVICE7		m_pD3DD;
		static LPDIRECTDRAWSURFACE7		m_pDDZBuffer;

		// PixelFormat
	    static DDPIXELFORMAT			m_PixelFormats[100]; // Stores pixel formats
	    static DWORD					m_dwNumPixelFormats;

		//------------------------------------------------------------------
		// Texture 생성 관련 정보
		//------------------------------------------------------------------
		// 하드웨어가 지원하는 Texture의 최대 크기
		static int						m_TextureWidthMax;
		static int						m_TextureHeightMax;

		static bool						m_bTexturePow2;		// 2^n만 되는가?
		static bool						m_bTextureSquareOnly;	// 정사각형만 되는가?

	public :
		// PixelFormat
		static DDPIXELFORMAT			m_PixelFormat4444;
		static DDPIXELFORMAT			m_PixelFormat1555;
};

#else
	// Non-Windows platforms: Direct3D not available
	// This file provides stub definitions for compilation only
	#include "CD3DMath.h"  // For D3DVECTOR, D3DMATRIX, D3DTLVERTEX types

	// Direct3D is not supported on non-Windows platforms
	// All CDirect3D functionality will be stubbed
	#pragma message("CDirect3D: Direct3D not supported on this platform - using stubs")

	// Define minimal types needed for stub
	#ifndef FALSE
		#define FALSE 0
		#define TRUE 1
	#endif
	typedef int BOOL;
	typedef unsigned char BYTE;
	typedef unsigned long DWORD;
	typedef unsigned short WORD;
	typedef long LONG;
	typedef int HRESULT;
	typedef void* HANDLE;

	// Rectangle structure
	typedef struct tagRECT {
		LONG left;
		LONG top;
		LONG right;
		LONG bottom;
	} RECT;

	// GUID structure
	typedef struct _GUID {
		DWORD Data1;
		WORD  Data2;
		WORD  Data3;
		BYTE  Data4[8];
	} GUID;

	// Pixel format structure
	typedef struct _DDPIXELFORMAT {
		DWORD dwSize;
		DWORD dwFlags;
		DWORD dwFourCC;
		union {
			DWORD  dwRGBBitCount;
			DWORD  dwYUVBitCount;
			DWORD  dwZBufferBitDepth;
			DWORD  dwAlphaBitDepth;
			DWORD  dwLuminanceBitCount;
			DWORD  dwBumpBitCount;
		};
		union {
			DWORD dwRBitMask;
			DWORD dwYBitMask;
			DWORD dwStencilBitDepth;
			DWORD dwLuminanceBitMask;
			DWORD dwBumpDuBitMask;
			DWORD dwOperations;
		};
		union {
			DWORD dwGBitMask;
			DWORD dwUBitMask;
			DWORD dwZBitMask;
			DWORD dwBumpDvBitMask;
			struct {
				WORD wFlipMSTypes;
				WORD wZFBSTypes;
			};
		};
		union {
			DWORD dwBBitMask;
			DWORD dwVBitMask;
			DWORD dwStencilBitMask;
			DWORD dwBumpLuminanceBitMask;
		};
		DWORD dwLuminanceAlphaBitMask;
	} DDPIXELFORMAT;

	// Material structure
	typedef struct _D3DMATERIAL7 {
		D3DVALUE   dvDiffuse;
		D3DVALUE   dvAmbient;
		D3DVALUE   dvSpecular;
		D3DVALUE   dvEmissive;
		D3DVALUE   dvPower;
		DWORD      dwRampSize;
	} D3DMATERIAL7;

	// Stub types for Direct3D
	typedef void* LPDIRECT3D7;
	typedef void* LPDIRECT3DDEVICE7;
	typedef void* LPDIRECTDRAWSURFACE7;
	typedef void* LPDDPIXELFORMAT;

	// Transform state constants
	#define D3DTRANSFORMSTATE_WORLD 1
	#define D3DPT_TRIANGLELIST 2
	#define D3DPT_TRIANGLESTRIP 3
	#define D3DFVF_VERTEX 4
	#define D3DFVF_TLVERTEX 5

	// Stub for CDirect3D - provides minimal interface for compilation
	class CDirect3D {
	public:
		CDirect3D() {}
		~CDirect3D() {}

		// Initialization stubs
		static bool Init() { return false; }
		static void Release() {}

		// Restore stubs
		static BOOL IsLost() { return FALSE; }
		static void Restore() {}

		// HAL check stubs
		static bool CheckHAL() { return false; }
		static bool IsHAL() { return false; }  // No hardware acceleration
		static bool IsAvailable() { return false; }

		// GetDevice stub - returns stub device pointer
		static LPDIRECT3DDEVICE7 GetDevice() { return NULL; }

		// Texture stubs
		static void GetTextureSize(int& width, int& height) { width = 256; height = 256; }
		static DWORD GetTextureWidthMax() { return 256; }
		static DWORD GetTextureHeightMax() { return 256; }
		static bool IsTexturePow2() { return true; }
		static bool IsTextureSquareOnly() { return false; }
	};
#endif
