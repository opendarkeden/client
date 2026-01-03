//----------------------------------------------------------------------
// CDirect3D.cpp
//----------------------------------------------------------------------

#include <math.h>
#include "CDirect3D.h"
#include "D3DUtil.h"
//#include "D3DTextr.h"
//#include "DebugInfo.h"

//----------------------------------------------------------------------
//
//                    Static members
//
//----------------------------------------------------------------------
bool					CDirect3D::m_bHAL		= false;

LPDIRECT3D7				CDirect3D::m_pD3D		= NULL;
LPDIRECT3DDEVICE7		CDirect3D::m_pD3DD		= NULL;
LPDIRECTDRAWSURFACE7	CDirect3D::m_pDDZBuffer	= NULL;

// Stores pixel formats
DDPIXELFORMAT			CDirect3D::m_PixelFormats[100]; 
DWORD					CDirect3D::m_dwNumPixelFormats = 0;

//----------------------------------------------------------------------
// Texture 생성 관련 정보
//----------------------------------------------------------------------
int						CDirect3D::m_TextureWidthMax	= 256;
int						CDirect3D::m_TextureHeightMax	= 256;
bool					CDirect3D::m_bTexturePow2		= true;			// 2^n만 되는가?		
bool					CDirect3D::m_bTextureSquareOnly	= true;	// 정사각형만 되는가?

// PixelFormat
DDPIXELFORMAT			CDirect3D::m_PixelFormat4444;
DDPIXELFORMAT			CDirect3D::m_PixelFormat1555;


//----------------------------------------------------------------------
// Callback Function
//----------------------------------------------------------------------
HRESULT WINAPI 
CDirect3D::EnumZBufferCallback(DDPIXELFORMAT* DDP_Format ,VOID* DDP_Desired)
{
	if(DDP_Format->dwFlags == DDPF_ZBUFFER)
	{
		memcpy( DDP_Desired, DDP_Format,sizeof(DDPIXELFORMAT) );
		return D3DENUMRET_CANCEL;
	}
	return D3DENUMRET_OK;
}

//-----------------------------------------------------------------------------
// Name: EnumTextureFormats()
// Desc: Stores enumerates texture formats in a global array.
//-----------------------------------------------------------------------------
HRESULT WINAPI 
CDirect3D::EnumTextureCallback( DDPIXELFORMAT* pddpf,
                                                      VOID* )
{
    m_PixelFormats[m_dwNumPixelFormats++] = (*pddpf);

    return D3DENUMRET_OK;
}

//-----------------------------------------------------------------------------
// Name: GetNumberOfBits()
// Desc: Returns the number of bits set in a DWORD mask
//-----------------------------------------------------------------------------
WORD 
CDirect3D::GetNumberOfBits( DWORD dwMask )
{
    for( WORD wBits = 0; dwMask; wBits++ )
        dwMask = dwMask & ( dwMask - 1 );  

    return wBits;
}

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
bool
CDirect3D::CheckHAL()
{
	m_bHAL = false;

	LPDIRECT3D7				pD3D = NULL;
	LPDIRECT3DDEVICE7		pD3DD = NULL;

	//--------------------------------------------------------
	// Query D3D Interface
	//--------------------------------------------------------
	if(m_pDD->QueryInterface (IID_IDirect3D7, (void **)&pD3D)==DD_OK)
	{		
		//--------------------------------------------------------
		// Create Device
		//--------------------------------------------------------
		if(pD3D->CreateDevice(IID_IDirect3DHALDevice, m_pDDSBack, &pD3DD)==D3D_OK)
		{
			m_bHAL = true;
		}
	}

	// Release
	if (pD3D) 
	{
		pD3D->Release();
	}

	if (pD3DD)
	{
		pD3DD->Release();
	}

	pD3D	= NULL;
	pD3DD	= NULL;	

	return m_bHAL;
}

//----------------------------------------------------------------------
// Init
//----------------------------------------------------------------------
bool
CDirect3D::Init()
{
	Release();

	// Hardware가속이 되는가?
	//m_Mode = MODE_HARDWARE;

	// 여기서 꼭 true해줘야지 Init()안에서 제대로 작동한다.
	m_bHAL		= true;

	if(!Init(NULL, &IID_IDirect3DHALDevice))
	{
		m_bHAL		= false;
		//m_Mode = MODE_MMX;
		Release();

		return false;

		/*
		// MMX가속이 되는가?
		if(!Init(NULL, &IID_IDirect3DMMXDevice))
		{
			//m_Mode = MODE_SOFTWARE;
			Release();

			// Software가속(?)은 되는가?
			if(!Init(NULL, &IID_IDirect3DRGBDevice))
			{
				//DirectDrawFailed("Couldn't initialize D3D");
				return false;
			}
		}
		*/
	}

	// Texture 생성에 관련된 정보를 읽어온다.
	if (!CheckDeviceForTexture())
	{
		Release();
		return false; 
	}

	return true;
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void 
CDirect3D::Release()
{
	m_bHAL		= false;

	if (m_pDDZBuffer) 
	{
		m_pDDZBuffer->Restore();	// --;;
		
		m_pDDSBack->DeleteAttachedSurface(0, m_pDDZBuffer);

		m_pDDZBuffer->Release();
		m_pDDZBuffer = NULL;
	}

	if(m_pD3DD)
	{
		m_pD3DD->Release();
		m_pD3DD = NULL;
	}

	if (m_pD3D) 
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	m_dwNumPixelFormats = 0;
}

//----------------------------------------------------------------------
// Init(....)
//----------------------------------------------------------------------
bool
CDirect3D::Init(GUID* pDriverGUID, const GUID* pDeviceGUID)
{
	DDSURFACEDESC2 ddsd;

	//--------------------------------------------------------
	// Query D3D Interface
	//--------------------------------------------------------
	if(m_pDD->QueryInterface (IID_IDirect3D7, (void **)&m_pD3D)!=DD_OK)
	{
		//DirectDrawFailed("Couldn't query D3D interface");
		return false;
	}

	//--------------------------------------------------------
	// Create Device
	//--------------------------------------------------------
	if(m_pD3D->CreateDevice(*pDeviceGUID, m_pDDSBack, &m_pD3DD)!=D3D_OK)
	{
		//DirectDrawFailed("Couldn't find any D3D Device");
		return false;
	}

	//--------------------------------------------------------
	// Enum Z-Buffer Formats
	//--------------------------------------------------------
	DDPIXELFORMAT DDPF_ZBuffer;
	m_pD3D->EnumZBufferFormats(*pDeviceGUID, EnumZBufferCallback, (VOID*)&DDPF_ZBuffer);

	if(sizeof(DDPIXELFORMAT)!=DDPF_ZBuffer.dwSize)
	{
		//DirectDrawFailed("couldn't enumerate ZBuffer formats for the device");
		return false;
	}

	//--------------------------------------------------------
	// Z-Buffer 관련 정보를 설정한다.
	//--------------------------------------------------------
	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
 
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;
	ddsd.dwWidth = m_ScreenWidth;
	ddsd.dwHeight = m_ScreenHeight;
	memcpy(&ddsd.ddpfPixelFormat, &DDPF_ZBuffer, sizeof(DDPIXELFORMAT));

	//--------------------------------------------------------
	// Hardware가속이 되는가? --> 되면 VideoMemory를 잡는다.
	//--------------------------------------------------------
	//if(IsEqualIID(*pDeviceGUID, IID_IDirect3DHALDevice)) 
	if (m_bHAL)
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
	}
	else 
	{
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}

	//--------------------------------------------------------
	// Z-Buffer Surface를 생성한다.
	//--------------------------------------------------------
	if(m_pDD->CreateSurface(&ddsd, &m_pDDZBuffer, NULL)!=DD_OK)
	{
		//DirectDrawFailed("Couldn't create zbuffer");
		return false;
	}

	m_pDDZBuffer->Restore();

	//--------------------------------------------------------
	// Z-Buffer Surface를 BackSurface에 붙인다.
	//--------------------------------------------------------
	if(m_pDDSBack->AddAttachedSurface(m_pDDZBuffer)!=DD_OK)
	{
		//DirectDrawFailed("Couldn't attach zbuffer to BackBuffer");
		return false;
	}

	//--------------------------------------------------------
	// BackSurface를 RenderTarget으로 설정
	//--------------------------------------------------------
	if(m_pD3DD->SetRenderTarget(m_pDDSBack, 0)!=D3D_OK)
	{
		//DirectDrawFailed("Couldn't set render target");
		return false;
	}


	//--------------------------------------------------------
	// TextureFormat찾기
	//--------------------------------------------------------	
    if(m_pD3DD->EnumTextureFormats( EnumTextureCallback, NULL )!=D3D_OK)
	{
		//DirectDrawFailed("Couldn't Enum Texture PixelFormats");
        return false;
	}

	//--------------------------------------------
	// PixelFormat찾기
	//--------------------------------------------
	if (!FindBestPixelFormat(4,4,4,4, &m_PixelFormat4444))
	{
//		DEBUG_ADD("[Error] Can't find 4444");
		//DirectDrawFailed("Initialize Failed - Couldn't Find PixelFormat4444");
		return false;
	}

	if (!FindBestPixelFormat(1,5,5,5, &m_PixelFormat1555))
	{
//		DEBUG_ADD("[Error] Can't find 1555");
		//DirectDrawFailed("Initialize Failed - Couldn't Find PixelFormat4444");
		return false;
	}

	//--------------------------------------------------------
	// Viewport를 지정한다.
	//--------------------------------------------------------
	D3DVIEWPORT7 vp = { 0, 0, m_ScreenWidth, m_ScreenHeight, 0.0f, 1.0f };

	if(m_pD3DD->SetViewport(&vp)!=D3D_OK)
	{
		//DirectDrawFailed("Couldn't set viewport");
		return false;
	}


	//--------------------------------------------------------
	//
	//--------------------------------------------------------
	D3DMATRIX mat;
	mat._11 = mat._22 = mat._33 = mat._44 = 1.0f;
	mat._12 = mat._13 = mat._14 = mat._41 = 0.0f;
	mat._21 = mat._23 = mat._24 = mat._42 = 0.0f;
	mat._31 = mat._32 = mat._34 = mat._43 = 0.0f;
			
	//--------------------------------------------------------
	// World
	//--------------------------------------------------------
	D3DMATRIX matWorld = mat;
	//matWorld._41 = x;	// 좌표
	//matWorld._42 = y;
	//matWorld._43 = z;
	m_pD3DD->SetTransform( D3DTRANSFORMSTATE_WORLD, &matWorld );

	//--------------------------------------------------------
	// View
	//--------------------------------------------------------
	D3DMATRIX matView = mat;
	matView._43 = 10.0f;
	m_pD3DD->SetTransform( D3DTRANSFORMSTATE_VIEW, &matView );

	//--------------------------------------------------------
	// Projection
	//--------------------------------------------------------
	D3DMATRIX matProj = mat;
	matProj._11 =  2.0f;		// 폭의 비율
	matProj._22 =  2.0f;		// 높이의 비율
	matProj._34 =  1.0f;
	matProj._43 = -1.0f;
	matProj._44 =  0.0f;
	m_pD3DD->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );


	//--------------------------------------------------------	
	// Material 설정
	//--------------------------------------------------------
	D3DMATERIAL7 mtrl;
    D3DUtil_InitMaterial( mtrl, 1.0f, 1.0f, 1.0f );
	m_pD3DD->SetMaterial( &mtrl );

	//--------------------------------------------------------
	// Light 설정
	//--------------------------------------------------------
    D3DLIGHT7 light;
    D3DUtil_InitLight( light, D3DLIGHT_POINT, 0.0f, 0.0f, -10.0f );
	//D3DUtil_InitLight( light, D3DLIGHT_POINT, 3.0f, 3.0f, 0.0f );	
	
	/*
	light.dltType      = D3DLIGHT_SPOT;
	light.dcvDiffuse.r   = 1.0f;
	light.dcvDiffuse.g   = 1.0f;
	light.dcvDiffuse.b   = 1.0f;
	light.dcvSpecular = light.dcvDiffuse;
	light.dvPosition.x = 0.0f;
	light.dvPosition.y = 0.0f;
	light.dvPosition.z = -10.0f;
	light.dvDirection.x = 0.0f;	
	light.dvDirection.y = 0.0f;
	light.dvDirection.z = 1.0f;
	light.dvTheta =       0.5f; //원뿔의 중앙 크기
	light.dvPhi =         0.8f; //원뿔의 외곽 크기
	light.dvAttenuation0 = 1.0f;
	light.dvFalloff		= 1.0f;
	*/

	m_pD3DD->SetLight( 0, &light );
    m_pD3DD->LightEnable( 0, TRUE );
    m_pD3DD->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );


	//--------------------------------------------------------
	// Rendering상태 설정하기..
	//--------------------------------------------------------
	m_pD3DD->SetRenderState( D3DRENDERSTATE_AMBIENT, 0x04040404 );
	m_pD3DD->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	m_pD3DD->SetRenderState( D3DRENDERSTATE_DITHERENABLE, TRUE );
    m_pD3DD->SetRenderState( D3DRENDERSTATE_SPECULARENABLE, FALSE );
	m_pD3DD->SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE );

	// Set Texture Stage
	m_pD3DD->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0);
	m_pD3DD->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DD->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//m_pD3DD->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//m_pD3DD->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_BLENDCURRENTALPHA);
	m_pD3DD->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTFN_LINEAR );
    m_pD3DD->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTFG_LINEAR );
	
	m_pD3DD->SetRenderState( D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_NONE);
	m_pD3DD->SetRenderState( D3DRENDERSTATE_EDGEANTIALIAS, FALSE);
	m_pD3DD->SetRenderState( D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);
	//m_pD3DD->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE );
	//m_pD3DD->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	m_pD3DD->SetRenderState( D3DRENDERSTATE_SRCBLEND,  D3DBLEND_SRCALPHA );
	m_pD3DD->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );   
	m_pD3DD->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
	//m_pD3DD->SetRenderState( D3DRENDERSTATE_ALPHAREF, 0x0F );
	m_pD3DD->SetRenderState( D3DRENDERSTATE_ALPHAREF, 0 );
	//m_pD3DD->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	m_pD3DD->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_NOTEQUAL );   
	m_pD3DD->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
	//m_pD3DD->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, TRUE );	     
	//m_pD3DD->SetRenderState( D3DRENDERSTATE_COLORKEYBLENDENABLE, TRUE );

//	DEBUG_ADD("CDirect3D::Init HAL OK");

	return true;
}

//----------------------------------------------------------------------
// Is Lost
//----------------------------------------------------------------------
BOOL				
CDirect3D::IsLost()
{
	if (m_pDDZBuffer!=NULL)
	{
		return m_pDDZBuffer->IsLost()!=DD_OK;
	}

	return FALSE;
}

//----------------------------------------------------------------------
// Restore
//----------------------------------------------------------------------
void				
CDirect3D::Restore()
{
	RestoreAllSurfaces();

	if (m_pDDZBuffer!=NULL)
	{
		m_pDDZBuffer->Restore();

		/*
		CDirectDraw::Flip();

		m_pDDZBuffer->Restore();

		CDirectDraw::Flip();

		m_pDDZBuffer->Restore();
		*/
	}
}

//----------------------------------------------------------------------
// Find Best PixelFormat
//----------------------------------------------------------------------
// Video Card가 지원하는 PixelFormat중에서
// A:R:G:B의 bit수가 같은걸 찾는다.
//----------------------------------------------------------------------
bool
CDirect3D::FindBestPixelFormat(int bitsA, int bitsR, int bitsG, int bitsB, 
								LPDDPIXELFORMAT pddpfBestMatch)
{
	for( DWORD i=0; i<m_dwNumPixelFormats; i++ )
    {
        DDPIXELFORMAT* pddpf = &m_PixelFormats[i];

        int nFormatABits = GetNumberOfBits( pddpf->dwRGBAlphaBitMask );
        int nFormatRBits = GetNumberOfBits( pddpf->dwRBitMask );
        int nFormatGBits = GetNumberOfBits( pddpf->dwGBitMask );
        int nFormatBBits = GetNumberOfBits( pddpf->dwBBitMask );

        if( pddpf->dwFourCC == 0 &&
            nFormatABits == bitsA &&
            nFormatRBits == bitsR &&
            nFormatGBits == bitsG &&
            nFormatBBits == bitsB ) 
        {
            // This is an exact pixel format match, so it works
            (*pddpfBestMatch) = (*pddpf);
            return true;
        }
    }

	return false;
}


//----------------------------------------------------------------------
// Check Device for Texture
//----------------------------------------------------------------------
bool
CDirect3D::CheckDeviceForTexture()
{
	//--------------------------------------------
	// TextureSurface의 Max크기에 대한 정보
	//--------------------------------------------
	// Get the device caps
    D3DDEVICEDESC7 ddDesc;
    if( FAILED( m_pD3DD->GetCaps( &ddDesc) ) )
        return false;

	// Limit max texture sizes, if the driver can't handle large textures
    m_TextureWidthMax  = (ddDesc.dwMaxTextureWidth)? ddDesc.dwMaxTextureWidth : 256;
    m_TextureHeightMax = (ddDesc.dwMaxTextureHeight)? ddDesc.dwMaxTextureHeight : 256;  

	// 2^n만 되는가?
	m_bTexturePow2 = (ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2)!=0;
		
	// 정사각형만 되는가?
	m_bTextureSquareOnly = (ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY)!=0;

	return true;
}

//----------------------------------------------------------------------
// Get Texture Size
//----------------------------------------------------------------------
// 하드웨어가 지원하는 범위내에서..
// 적절한 Texture의 크기를 결정한다.
//----------------------------------------------------------------------
void
CDirect3D::GetTextureSize(int& width, int& height)
{
	//---------------------------------------------------------------
	// 하드웨어가 지원하는 TextureSurface의 크기를 결정한다.
	//---------------------------------------------------------------
	// Adjust width and height to be powers of 2, if the device requires it	
	if( m_bTexturePow2 )
	{
		int w, h;

		for( w=1;  width>w;   w<<=1 );
		for( h=1; height>h; h<<=1 );

		width  = min( w,  m_TextureWidthMax );
		height = min( h, m_TextureHeightMax );
	}
	else
	{
		width  = min( width,  m_TextureWidthMax );
		height = min( height, m_TextureHeightMax );
	}	

	// Make the texture square, if the driver requires it
	if( m_bTextureSquareOnly )
	{
		if( width > height ) 
			height = width;
		else	
			width  = height;
	}
}