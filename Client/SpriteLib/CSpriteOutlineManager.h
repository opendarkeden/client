//----------------------------------------------------------------------
// CSpriteOutlineManager.h
//----------------------------------------------------------------------
/*
                    <<<   외곽선 출력하기   >>>



	여러개의 Sprite가 겹쳐서 출력될 경우... 
	Sprite들이 다 출력된 후의 외곽선을 추출해야 한다.
	(캐릭터 몸 + 복장들 + 무기...)
	SpriteOutlineManager class를 만들어서 이용한다.


	(실행 예제)

	  SpriteOutlineManager SOM;

	  // 출력할려는 Sprite를 SpriteOutlineManager에 등록한다.
	  SOM.Add( x0,y0, 캐릭터몸 );
	  SOM.Add( x1,y1, 캐릭터복장1 );
	  SOM.Add( x2,y2, 캐릭터복장2 );
	  SOM.Add( x3,y3, 캐릭터무기 );

	  // 출력한다.
	  SOM.BltOutline( CDirectDrawSurface* );



	(내부 구현)

	Add( Sprite* )
	  - Sprite를 겹쳐서 출력했을 경우의 최종 상태에서의 
		Sprite가 출력된 영역의 크기를 구해야 하므로
		Sprite Size를 읽어서 MaximumRect를 계산해둔다.
		이 때, Sprite가 출력되는 위치를 잘 따져봐야 한다. (음수 주의)
	  - Sprite List에 Sprite를 추가한다.


	BltOutline( CDirectDrawSurface* )
	  - MaximumRect는 외곽선 때문에
		실제크기보다 사방으로 1 Pixel씩 커져야 한다.
 
	  - 외곽선을 추출하기 위해서는.. 
		투명색과 실제색이 있는 부분을 구분해야 하므로
		각 Sprite의 GetPixelInfo( PixelInfo*, ColorInfo* )를 이용해서
		SpriteOutlineManager 내부의 PixelInfo에 Sprite의 투명색+색깔의
		정보를 저장하고 ColorInfo에는 각 Sprite의 색깔 값을 저장한다.
		PixelInfo값은 0(투명색)과 1(색깔)로 구분되고 ColorInfo값은
		실제 색깔 값을 의미한다.
		PixelInfo는 SpriteOutlineManager가 생성될 때 0으로 
		초기화해야 되며, Sprite의 GetPixelInfo에서는 색깔부분만 1로
		만들어줘야 한다.

	  - 생성된 PixelInfo와 ColorInfo의 정보를 이용해서
		Surface에 외곽선과 함께, 실제 색깔을 출력해야 한다.

	  - 일단 PixelInfo의 0(투명)과 1(색깔)을 검색하면서 
		외곽선 부분에는 2(외곽)를 대입해준다.
		세로 외곽 검색은 해줘야 하고,
		가로 외곽 검색은 Add에서 해줄 수도 있다.
		하지만, 가로 외곽 검색을 Add에서 하지 않는 다면, 
		이 단계에서 가로로 검색하면서 투명색부분 압축을 해서
		잠시 후의 출력에서 그 정보를 활용할 수 있다.
         
	  - 출력 할때는 PixelInfo의 값에 따라서
		투명 부분은 건너띄고,
		외곽선 부분은 외곽선 색을 출력하고,
		색깔부분은 정해진 색깔을 출력한다.
*/
//----------------------------------------------------------------------


#ifndef	__CSPRITEOUTLINEMANAGER_H__
#define	__CSPRITEOUTLINEMANAGER_H__

// 외곽선을 출력할때 겹쳐지는 Sprite들의 최대 개수
#define	MAX_SPRITE_MERGE	20

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include "../basic/Platform.h"
#endif
class CSprite;
class CAlphaSprite;
class CIndexSprite;

class CSpriteOutlineManager {
	public :
		CSpriteOutlineManager();
		~CSpriteOutlineManager();

		bool		IsInit()	{ return m_nSprite!=0; }

		//--------------------------------------------------------------
		// Clear
		//--------------------------------------------------------------
		void		Clear();		

		//--------------------------------------------------------------
		// Add Sprite
		//--------------------------------------------------------------
		void		Add(int x, int y, CSprite* pSprite);
		void		Add(int x, int y, CIndexSprite* pSprite, WORD colorSet=0xFFFF);
		void		Add(int x, int y, CAlphaSprite* pSprite);

		//--------------------------------------------------------------
		// Generate  : 외곽선 정보를 생성한다.
		//--------------------------------------------------------------
		void		Generate(int opt = 0);

		//--------------------------------------------------------------
		// Get
		//--------------------------------------------------------------
		// 출력 위치
		POINT		GetPosition() const		{ return m_pointOutput; }
		// 외곽선 정보의 크기
		int			GetWidth() const		{ return m_Width; }
		int			GetHeight() const		{ return m_Height; }

		//--------------------------------------------------------------
		// Blt  : 화면에 출력한다.
		//--------------------------------------------------------------
		void		Blt(WORD* pSurface, WORD Pitch, WORD color);
		void		BltClip(WORD* pSurface, WORD Pitch, WORD color, RECT* pRect);
		void		BltOutline(WORD* pSurface, WORD Pitch, WORD color);
		void		BltOutlineClip(WORD* pSurface, WORD Pitch, WORD color, RECT* pRect);
		void		BltDarkness(WORD* pSurface, WORD Pitch, WORD color, BYTE DarkBits);
		void		BltDarknessClip(WORD* pSurface, WORD Pitch, WORD color, BYTE DarkBits, RECT* pRect);

	public :
		// Sprite의 종류
		enum SPRITETYPE 
		{
			SPRITETYPE_NULL = 0,
			SPRITETYPE_NORMAL,
			SPRITETYPE_ALPHA,
			SPRITETYPE_INDEX
		};

		// 한 점의 종류(m_ppPixelInfo에서 사용)
		enum PIXELTYPE
		{			
			PIXELTYPE_MAXCOUNT = 0xF0,
			PIXELTYPE_TRANS = 0xFC,
			PIXELTYPE_COLOR = 0xFD,
			PIXELTYPE_OUTLINE = 0xFE,
			PIXELTYPE_TRANSEND = 0xFF
		};
		// 2004, 8, 18 sobeit add start - 공성전 성문 타겟팅때문에 여차여차 추가..
		enum GENERATE_OPTION{
			GENERATE_ALL = 0,
			GENERATE_EXCEPT_LEFT,	// 왼쪽 아웃라인은 제외
			GENERATE_EXCEPT_RIGHT,  // 오른쪽 아웃라인은 제외
			GENERATE_EXCEPT_SIDE,	// 양 사이드 아웃라인은 제외
			// -_- 위,아래도 만들어 놔야 하나..-_-;
		};
		// 2004, 8, 18 sobeit add start
	
	protected :
		//--------------------------------------------------------------
		// Init/Release
		//--------------------------------------------------------------
		void		InitInfo(int width, int height);
		void		ReleaseInfo();

		//--------------------------------------------------------------
		// Sprite의 Pixel정보를 외곽선 정보에 추가
		//--------------------------------------------------------------
		void		MergeNormalSprite( int n );
		void		MergeAlphaSprite( int n );
		void		MergeIndexSprite( int n );
		void		MergeIndexSpriteColorSet( int n );	// 전체 색깔이 바뀌는 IndexSprite


	protected :
		int			m_nSprite;						// 겹쳐찍는 Sprite개수
		int			m_Type[MAX_SPRITE_MERGE];		// Sprite의 종류		
		POINT		m_Position[MAX_SPRITE_MERGE];	// Sprite를 출력할 위치
		WORD		m_ChangeColorSet[MAX_SPRITE_MERGE];	// IndexSprite의 전체색깔이 바뀔때의 Set값
		WORD		m_Value[MAX_SPRITE_MERGE][256];		// IndexSprite의 Set값
		void*		m_pSprite[MAX_SPRITE_MERGE];	// Sprite에 대한 pointer

		RECT		m_rectOutline;					// 전체 영역
		POINT		m_pointOutput;					// 출력되는 위치

		// 외곽선 정보
		int			m_Width;						// 가로 길이
		int			m_Height;						// 세로 길이
		BYTE**		m_ppPixelInfo;					// pixel의 종류
		WORD**		m_ppColorInfo;					// pixel의 색깔
};

#endif

