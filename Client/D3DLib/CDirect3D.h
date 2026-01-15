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

// Debug: Check if PLATFORM_WINDOWS is defined
#ifdef PLATFORM_WINDOWS
#pragma message("CDirect3D: PLATFORM_WINDOWS IS DEFINED - using Windows path")
#else
#pragma message("CDirect3D: PLATFORM_WINDOWS is NOT defined - using non-Windows path")
#endif

#ifdef PLATFORM_WINDOWS
	#include <D3D.h>
	#include <ddraw.h>

	//#include "d3dtextr.h"
	//#include "d3dutil.h"

	// #include "CDirectDraw.h"  // Removed - not needed for stub compilation




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
	#include "basic/Platform.h"  // For DWORD, etc.
	#include "CD3DMath.h"  // For D3DVECTOR, D3DMATRIX, D3DTLVERTEX types

	// Direct3D is not supported on non-Windows platforms
	// All CDirect3D functionality will be stubbed
	#pragma message("CDirect3D: Direct3D not supported on this platform - using stubs")

	// HRESULT constants
	#ifndef D3D_OK
		#define D3D_OK 0
	#endif
	#ifndef D3DERR_INVALIDCALL
		#define D3DERR_INVALIDCALL (-1)
	#endif

	// Define stub types for compilation only
	// These are opaque pointer types for non-Windows platforms
	#ifndef LPDIRECT3D7
		typedef void* LPDIRECT3D7;
	#endif

	// DDPIXELFORMAT structure - get it from CDirectDraw.h
	// We must include CDirectDraw.h to get the proper definition that's
	// consistent with the dxlib library compilation
	#include "../DXLib/CDirectDraw.h"

	// D3DPRIMITIVETYPE - use DWORD instead of enum for implicit int conversion
	#ifndef D3DPRIMITIVETYPE
		typedef DWORD D3DPRIMITIVETYPE;
		#define D3DPT_POINTLIST 1
		#define D3DPT_LINELIST 2
		#define D3DPT_LINESTRIP 3
		#define D3DPT_TRIANGLELIST 4
		#define D3DPT_TRIANGLESTRIP 5
		#define D3DPT_TRIANGLEFAN 6
	#endif

	// D3DTRANSFORMSTATETYPE - use DWORD instead of enum for implicit int conversion
	#ifndef D3DTRANSFORMSTATETYPE
		typedef DWORD D3DTRANSFORMSTATETYPE;
		#define D3DTRANSFORMSTATE_WORLD 1
		#define D3DTRANSFORMSTATE_VIEW 2
		#define D3DTRANSFORMSTATE_PROJECTION 3
		#define D3DTRANSFORMSTATE_WORLD1 4
		#define D3DTRANSFORMSTATE_WORLD2 5
		#define D3DTRANSFORMSTATE_WORLD3 6
	#endif

	// LPWORD pointer type
	#ifndef LPWORD
		typedef WORD* LPWORD;
	#endif

	// D3DMATERIAL7 stub (must be defined before IDirect3DDevice7)
	#ifndef D3DMATERIAL7_DEFINED
	#define D3DMATERIAL7_DEFINED
	typedef struct D3DMATERIAL7 {
		D3DVALUE dvDiffuse;
		D3DVALUE dvAmbient;
		D3DVALUE dvSpecular;
		D3DVALUE dvEmissive;
		D3DVALUE dvPower;
		DWORD dwRampSize;
	} D3DMATERIAL7, *LPD3DMATERIAL7;
	#endif

	// FVF (Flexible Vertex Format) constants
	#ifndef D3DFVF_VERTEX
		#define D3DFVF_VERTEX 0x004
		#define D3DFVF_TLVERTEX 0x030
	#endif

	// D3DDEVICEDESC7 structure stub
	#ifndef D3DDEVICEDESC7_DEFINED
	#define D3DDEVICEDESC7_DEFINED
	typedef struct _D3DDEVICEDESC7 {
		DWORD dwSize;
		// Stub - just need the type for compilation
	} D3DDEVICEDESC7, *LPD3DDEVICEDESC7;
	#endif

	// D3D texture capability flags
	#ifndef D3DPTEXTURECAPS_POW2
		#define D3DPTEXTURECAPS_POW2 0x00000001
		#define D3DPTEXTURECAPS_SQUAREONLY 0x00000002
		#define D3DPTEXTURECAPS_ALPHAPALETTE 0x00000004
	#endif

	// Stub IDirect3DDevice7 class for compilation on non-Windows platforms
	// This provides the interface methods that CD3DObject.cpp uses
	class IDirect3DDevice7 {
	public:
		// Stub methods - these do nothing on non-Windows platforms
		HRESULT SetMaterial(LPD3DMATERIAL7 lpMaterial) { return D3D_OK; }
		HRESULT GetTransform(D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix) { return D3D_OK; }
		HRESULT SetTransform(D3DTRANSFORMSTATETYPE dtstTransformStateType, LPD3DMATRIX lpD3DMatrix) { return D3D_OK; }
		HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE d3dptPrimitiveType, DWORD dwVertexTypeDesc,
			LPVOID lpvVertices, DWORD dwVertexCount, LPWORD dwIndices, DWORD dwIndexCount, DWORD dwFlags) { return D3D_OK; }
		HRESULT DrawPrimitive(D3DPRIMITIVETYPE d3dptPrimitiveType, DWORD dwVertexTypeDesc,
			LPVOID lpvVertices, DWORD dwVertexCount, DWORD dwFlags) { return D3D_OK; }
		HRESULT BeginScene() { return D3D_OK; }
		HRESULT EndScene() { return D3D_OK; }
		HRESULT SetTexture(DWORD dwStage, struct IDirectDrawSurface7* lpTexture) { return D3D_OK; }
		HRESULT SetRenderState(DWORD dwRenderStateType, DWORD dwRenderState) { return D3D_OK; }

		// GetCaps stub - returns D3D_OK but doesn't fill in structure
		HRESULT GetCaps(void* pD3DHWDevDesc, void* pD3DHELDevDesc) {
			(void)pD3DHWDevDesc; (void)pD3DHELDevDesc;
			return D3D_OK;
		}
	};

	#ifndef LPDIRECT3DDEVICE7
		typedef IDirect3DDevice7* LPDIRECT3DDEVICE7;
	#endif

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
		static LPDIRECT3DDEVICE7 GetDevice() {
			static IDirect3DDevice7 s_stubDevice;
			return &s_stubDevice;
		}

		// Texture stubs
		static void GetTextureSize(int& width, int& height) { width = 256; height = 256; }
		static DWORD GetTextureWidthMax() { return 256; }
		static DWORD GetTextureHeightMax() { return 256; }
		static bool IsTexturePow2() { return true; }
		static bool IsTextureSquareOnly() { return false; }

		// PixelFormat stubs
		static const LPDDPIXELFORMAT GetPixelFormat4444() {
			static DDPIXELFORMAT s_fmt4444 = { sizeof(DDPIXELFORMAT) };
			return &s_fmt4444;
		}
		static const LPDDPIXELFORMAT GetPixelFormat1555() {
			static DDPIXELFORMAT s_fmt1555 = { sizeof(DDPIXELFORMAT) };
			return &s_fmt1555;
		}
	};
#endif /* PLATFORM_WINDOWS */

#endif /* __CDIRECT3D_H__ */
