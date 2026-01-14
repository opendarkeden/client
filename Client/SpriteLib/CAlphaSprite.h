//----------------------------------------------------------------------
// CAlphaSprite.h
//----------------------------------------------------------------------

#ifndef	__CALPHASPRITE_H__
#define	__CALPHASPRITE_H__

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include "../basic/Platform.h"
#endif

#ifdef SPRITELIB_BACKEND_SDL
#include "SpriteLibBackend.h"
#endif

std::ofstream;
std::ifstream;


//----------------------------------------------------------------------
// width * height��ŭ�� pixel�� �����صд�.
//----------------------------------------------------------------------
// �� pixel�� �����ϴ� ����� ������ ����.
//
//    [1] 5:5:5 --> 2 bytes
//    [2] 5:6:5 --> 2 bytes
//    [3] R,G,B --> 3 bytes
//
// [3]�� �뷮�� Ŀ�� �������Ƿ� [2]�� ����� ����ؼ�
// 5:5:5�� ���� ������� ����ǵ��� �Ѵ�.
//
// Memory�� 5:5:5�� 5:6:5�� ���������
// disk���� 5:6:5�� ����ȴ�.
//
// ��, 5:5:5�� ����ϴ� system������ 
//    File(5:6:5) ---(5:5:5�� ��ȯ)--> Memory(5:5:5)
//    File(5:6:5) <--(5:6:5�� ��ȯ)--- Memory(5:5:5)  �̷��� �ؾߵȴ�.
//
//----------------------------------------------------------------------
// ���������� 0�� ������ ����Ѵ�.
//
// = �� ���� ����
//
// (�ݺ�ȸ��) (������, �����, �����(alpha,����,alpha,����...)) (������, �����, �����(alpha,����,alpha,����...)...) ..
//
//
// Alpha���� ������ ���� �����Ѵ�.
//----------------------------------------------------------------------

class CAlphaSprite
{
	public :
		CAlphaSprite();
		~CAlphaSprite();

		bool		IsNotInit() const	{ return !m_bInit; }
		bool		IsInit() const		{ return m_bInit; }

		void		operator = (const CAlphaSprite& Sprite);

		//---------------------------------------------------------
		// m_Pixels�� memory�� Release�Ѵ�.		
		//---------------------------------------------------------
		void		Release();

		//---------------------------------------------------------
		// ���� Color 
		//---------------------------------------------------------
		static void	SetColorkey(WORD color)			{ s_Colorkey = color; }
		static WORD	GetColorkey() 					{ return s_Colorkey; }

		//---------------------------------------------------------
		// fstream���� save/load�� �Ѵ�.
		//---------------------------------------------------------
		virtual bool		SaveToFile(std::ofstream& file) = 0;
		virtual bool		LoadFromFile(std::ifstream& file) = 0;		
		//virtual bool		LoadFromFileToBuffer(ifstream& file) = 0;
		//void		LoadFromBuffer();
			
		//---------------------------------------------------------
		// CDirectDrawSurface�� ������ �о m_Pixels�� �����Ѵ�.
		//---------------------------------------------------------
		void		SetPixel(WORD* pSource, WORD sourcePitch, 
							 WORD* pFilter, WORD filterPitch, 
							 WORD width, WORD height);

		// (x,y)�� sprite�� ���� �κ��ΰ�?
		bool		IsColorPixel(short x, short y);

		//---------------------------------------------------------
		// get functions		
		//---------------------------------------------------------
		WORD		GetWidth()		const			{ return m_Width; }
		WORD		GetHeight()		const			{ return m_Height; }
		WORD		GetPixel(int x, int y, int bColor=1) const;
		WORD*		GetPixelLine(WORD y)	const	{ return m_Pixels[y]; }

#ifdef SPRITELIB_BACKEND_SDL
		/* Backend sprite management */
		spritectl_sprite_t GetBackendSprite() const	{ return m_backend_sprite; }
		bool IsBackendDirty() const			{ return m_backend_dirty; }
		void SetBackendSprite(spritectl_sprite_t sprite)	{ m_backend_sprite = sprite; }
		void SetBackendDirty(bool dirty)		{ m_backend_dirty = dirty; }
#endif

		//---------------------------------------------------------
		//
		// Blt
		//
		//---------------------------------------------------------

		//---------------------------------------------------------
		// �������� Blt
		//---------------------------------------------------------
		void		Blt(WORD *pDest, WORD pitch);				
		void		BltClip(WORD* pDest, WORD pitch, RECT* pRect);
		void		BltClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		BltClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Blt 4444  for Texture
		//---------------------------------------------------------
		void		Blt4444(WORD *pDest, WORD pitch);				
		void		Blt4444ClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444ClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444ClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444ClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Blt 4444 NotTrans for Texture (�����κе� �˰� ĥ�Ѵ�)
		//---------------------------------------------------------
		void		Blt4444NotTrans(WORD *pDest, WORD pitch);				
		void		Blt4444NotTransClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444NotTransClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444NotTransClipWidth(WORD *pDest, WORD pitch, RECT* pRect);
		void		Blt4444NotTransClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// shift��ŭ shift�ؼ� ũ�⸦ �۰� ��½�Ų��.
		//---------------------------------------------------------
		void		Blt4444SmallNotTrans(WORD *pDest, WORD pitch, BYTE shift);

		//---------------------------------------------------------
		// �¿� �ٲ�
		//---------------------------------------------------------
	

		//---------------------------------------------------------
		// ������
		//---------------------------------------------------------
		//void		BltHalf(WORD *pDest, WORD pitch);		
		//void		BltHalfClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltHalfClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltHalfClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// Alpha Blending
		//---------------------------------------------------------
		void		BltAlpha(WORD *pDest, WORD pitch, BYTE alpha);
		void		BltAlphaClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipWidth(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);
		void		BltAlphaClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE alpha);

		//---------------------------------------------------------
		// RGB�� �ٲ�
		//---------------------------------------------------------
		//void		BltColor(WORD *pDest, WORD pitch, BYTE rgb);		
		//void		BltColorClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		//void		BltColorClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);
		//void		BltColorClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE rgb);

		//---------------------------------------------------------
		// ��Ӱ� �ϱ�
		//---------------------------------------------------------
		//void		BltDarkness(WORD *pDest, WORD pitch, BYTE DarkBits);		
		//void		BltDarknessClipLeft(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		//void		BltDarknessClipRight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);
		//void		BltDarknessClipHeight(WORD *pDest, WORD pitch, RECT* pRect, BYTE DarkBits);

		//---------------------------------------------------------
		// Effect
		//---------------------------------------------------------
		//void		BltEffect(WORD *pDest, WORD pitch);				
		//void		BltEffectClipLeft(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltEffectClipRight(WORD *pDest, WORD pitch, RECT* pRect);
		//void		BltEffectClipHeight(WORD *pDest, WORD pitch, RECT* pRect);

		//---------------------------------------------------------
		// AlphaChannel Filter�� �̿��� ���
		//---------------------------------------------------------
		//void		BltAlphaFilter(WORD *pDest, WORD pitch, CFilter* pFilter);		

		//---------------------------------------------------------
		// Utility Functions
		//---------------------------------------------------------
		//void			memcpyHalf(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlpha(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlpha4444(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlpha4444Small(WORD* pDest, WORD* pSource, WORD pixels);
		void		memcpyAlphaValue(WORD* pDest, WORD* pSource, WORD pixels);
		//void			memcpyColor(WORD* pDest, WORD* pSource, WORD pixels);
		//void			memcpyDarkness(WORD* pDest, WORD* pSource, WORD pixels);

		//void			memcpyEffect(WORD* pDest, WORD* pSource, WORD pixels);
		//void		memcpyDarknessFilter(WORD* pDest, WORD* pSource, WORD* pFilter, WORD pixels);


		//---------------------------------------------------------
		// debug function
		//---------------------------------------------------------
		//static void		OutputLog()
		//{
		//	ofstream file("log.txt",ios::app);
		//	file << "New     = " << s_New << endl;
		//	file << "Delete  = " << s_Delete << endl;
		//	file << "------------------------" << endl;
		//}


	protected :
		WORD			m_Width;		// ���� pixel��
		WORD			m_Height;		// ���� pixel��
		WORD**			m_Pixels;		// pixels
		bool			m_bInit;		// data�� �ִ°�?

#ifdef SPRITELIB_BACKEND_SDL
		spritectl_sprite_t	m_backend_sprite;	// Backend sprite handle
		bool			m_backend_dirty;	// True if m_Pixels changed but not synced to backend
#endif

		static WORD		s_Colorkey;

		// Blt Value
		static int		s_Value1;
		static int		s_Value2;

};


#endif

