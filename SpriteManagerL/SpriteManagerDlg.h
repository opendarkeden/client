// SpriteManagerDlg.h : header file
//

#if !defined(AFX_SPRITEMANAGERDLG_H__90E8B5C8_45C9_11D4_AC04_0050DABAC3D4__INCLUDED_)
#define AFX_SPRITEMANAGERDLG_H__90E8B5C8_45C9_11D4_AC04_0050DABAC3D4__INCLUDED_

#pragma once


#include <vector>

#include "AniDlg.h"
#include "CDib.h"
#include "DIBList.h"

#include "CUndo.h"


enum DIRECTION
{
	DIR_LEFT,
	DIR_LEFTDOWN,
	DIR_DOWN,
	DIR_RIGHTDOWN,
	DIR_RIGHT,
	DIR_RIGHTUP,
	DIR_UP,
	DIR_LEFTUP,
	DIR_MAX,
};

#define DEFAULT_XOFFSET		90
#define DEFAULT_YOFFSET		160




#define DATAFILE_NAME		"spr.dat"


#define ANIMATION_SECTION	0
#define IMAGE_SECTION		1

const DWORD imageGap = 10;	// image사이의 여백.

#define TIMER_ID	1818
#define DEFAULT_FPS			16



// !주의! creature의 수가 변경되면 아래의 define도 변경되야 한다.
enum CREATURE_TYPE {
	TYPE_SLAYER_MAN	= 0,
	TYPE_SLAYER_WOMAN,
	TYPE_VAMPIRE_PC_MAN,	//  vampire중 처음이어야만 한다.
	TYPE_VAMPIRE_PC_WOMAN,
	TYPE_NPC,				// npc를 위한 공통 공간.
	TYPE_MIDLEVEL_MOB,		// 무서운 중급몹들..
	TYPE_OUSTERS,			// 아우스터즈

	TYPE_PROMOTION_SLAYER_MAN,
	TYPE_PROMOTION_SLAYER_WOMAN,
	TYPE_PROMOTION_VAMPIRE_PC_MAN,
	TYPE_PROMOTION_VAMPIRE_PC_WOMAN,
	TYPE_PROMOTION_OUSTERS,
	
	TYPE_MAX,
};
#define VAMPIRES		1	// 타입중 vampire의 수.
#define NPC				1
#define TYPE_VAMPIRE	TYPE_VAMPIRE_PC_MAN	// vampire전체를 대표하는 define..


//#define _TEMP_FOR_INTERFACE

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 수정시 SpriteManagerDlg.cpp의 partType전역 변수도 수정해줘야 함다.
//

#define NPC_ACTION_MAX			8					// 정지, 걷기, 데미지, 흡혈당하기, 죽기, 정지2???

#include "ActionEnum.h"

#define SLAYER_ACTION_MAX		(ACTION_MAX-1)		// slayer의 action 수.(-4는 뱀파attack attack &slow & fast, drain, npcstop의 제외)
#define MIDLEVEL_MOB_ACTION_MAX			VAMPIRE_ACTION_MAX					// 중급몹..(-4는 뱀파 attack&skill slow &fast 제외)
#define VAMPIRE_ACTION_MAX		(ACTION_DRAIN+1)	// vampire의 action의 수.

// 아우스터즈
extern const char* 	strOustersAction[ OUSTERS_ACTION_MAX ];
extern const char* strOustersActionForSearch[ OUSTERS_ACTION_MAX ];
extern const char* strOustersPart[OUSTERS_PART_MAX];
extern const DWORD oustersActionIndex[ OUSTERS_ACTION_MAX ];
extern const char* strPromotionOustersPart[PO_PART_MAX];

// 전직 슬
extern const char* strPromotionSlayerAction[ ACTION_SLAYER_PROMOTION_MAX ];
extern const char* strPromotionSlayerActionForSearch[ ACTION_SLAYER_PROMOTION_MAX ];
extern const DWORD g_PromotionSlayerActionIndex[ ACTION_SLAYER_PROMOTION_MAX ];
extern const char* strPromotionSlayerPart[ PS_PART_MAX ];

// 일반 슬
extern const BYTE s_AddonOrder[DIR_MAX][ADDON_MAX];
extern const DWORD slayerActionIndex[ SLAYER_ACTION_MAX];
extern const char* strSlayerPart[SLAYER_PART_MAX];
extern const char* strActionForSearch[ ACTION_MAX];
extern const char* strAction[ ACTION_MAX ];
extern const BYTE SlayerPartType[SLAYER_PART_MAX];

// 뱀파/npc
extern const DWORD npcActionIndex[ NPC_ACTION_MAX];
extern const DWORD vampireActionIndex[ VAMPIRE_ACTION_MAX];
extern const char* strVampirePart[VAMPIRE_PART_MAX];
extern const BYTE ACSlayerPartType[ PS_PART_MAX ];
	
#define MAX_ANIS_FRAME		500		// animation frame의 최대수..


BOOL IsSlayerType( int type);
BOOL IsVampireType( int type);
BOOL IsNPCType(int type);
BOOL IsMidLevelMobType(int type);
BOOL IsPromotionSlayer( int type );
BOOL IsPromotionVampire( int type );
BOOL IsPromotionOusters( int type );

#define BIKE_FRAME_COUNT 4



/////////////////////////////////////////////////////////////////////////////
// CSpriteManagerDlg dialog

class CSpriteManagerDlg : public CDialog
{
// Construction
public:
	CSpriteManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSpriteManagerDlg)
	enum { IDD = IDD_SPRITEMANAGER_DIALOG };
	CListBox	m_AniList;
	CComboBox	m_comboPart;
	CSliderCtrl	m_imageSliders;
	CSliderCtrl	m_aniSlider;
	CComboBox	m_comboType;
	CComboBox	m_comboDirection;
	CComboBox	m_comboAction;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpriteManagerDlg)
	public:
	virtual BOOL DestroyWindow();
	afx_msg void OnSelchangeComboDirection();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpriteManagerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonPlay();
	afx_msg void OnMenuDeleteAllAnis();
	afx_msg void OnMenuDeleteAni();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonDirectory();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboAction();
	afx_msg void OnSelchangeComboPart();
	afx_msg void OnButtonSearch();
	afx_msg void OnMenuSend();
	afx_msg void OnMenuSendAll();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonMakeSprite();
	afx_msg void OnButtonLoad();
	afx_msg void OnClose();
	afx_msg void OnRepeatCopy();
	afx_msg void OnButton1();
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	afx_msg void OnButtonUndo();
	afx_msg void OnButtonRedo();
	afx_msg void OnCopy();
	afx_msg void OnPaste();
	afx_msg void OnSelchangeAnimationList();
	afx_msg void OnListup();
	afx_msg void OnListdown();
	afx_msg void OnListdelete();
	afx_msg void OnListcopy();
	afx_msg void OnListpaste();
	afx_msg void OnListcut();
	afx_msg void OnListadd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
 
public:
	void LoadDataFile();
	void SearchBMP( CString& str, BOOL bUpdateAniData, BOOL bUpdateShadow = FALSE);
	void MakeSprite( DWORD type, CString& strData, CString fileName);
	void MakeShadowSprite( DWORD type, CString& strData, CString fileName);
	CString GetSearchString( DWORD type);
	void UpdateInfo();
	void SaveDataFile();
	int AddToAni( LPCSTR filename, int index, BOOL bAfter = TRUE);
	bool isBodyPart( int);
	CString GetSearchString( DWORD type, DWORD action, DWORD part, DWORD direction);
//	CString GetSearchString( DWORD type, DWORD action, DWORD part = PART_BIKE-1);
	CString GetSearchString( DWORD type, DWORD action, DWORD part);
	void FlushAniList();
	void FlushImageList();
	void LoadCurrentSet();
	void SaveCurrentSet();
	void ResetCombos();
//	void DeleteAni( int id);
	void InvalidateImage( BOOL bErase = TRUE);
	void InvalidateAni( BOOL bErase = FALSE);
	int GetImgID( const CPoint& point, int& imgID, RECT* pRect = NULL);
	void DoSpecialAction( DWORD type);


	float					m_fSizeRatio;		// 축소비


	CDIBList				m_imageList;
												// 즉, 실제 ani의 순서

	RECT					m_imageRect;
	RECT					m_aniRect;

	RECT					m_imageSection;
	RECT					m_aniSection;

	RECT					m_selectionRect;

	int						m_iSelectedImageID;
	int						m_iSelectedAniID;
	BOOL					m_bDragging;
	BOOL					m_bDrawSelectionRect;
	int						m_iDragFrom;

	BOOL					m_bConfirmDelete;
	BOOL					m_bExtended;

	bool					m_bUndoStop;	//Undo기능을 잠시 정지시킨다.

	class MyData
	{
	public:
		int 					m_iAniIndex[MAX_ANIS_FRAME];	// m_aniList의 index를 가지고 있다.
		char					m_pCopyFlag[ MAX_ANIS_FRAME];
		int						m_iAniLength;
		int						m_iBmpsCount;
		CDIBList				m_aniList;

		void operator=( const MyData& s)
		{
			memcpy( m_iAniIndex, s.m_iAniIndex, sizeof( m_iAniIndex));
			memcpy( m_pCopyFlag, s.m_pCopyFlag, sizeof( m_pCopyFlag));
			m_iAniLength = s.m_iAniLength;
			m_iBmpsCount = s.m_iBmpsCount;
			m_aniList = s.m_aniList;
		}
	} m_myData;

	CUndo<MyData, 10>		m_undo;

	DWORD					m_dwType, m_dwAction, m_dwPart, m_dwDirection;
	char					m_strDirectoryName[ MAX_PATH];

	HCURSOR					m_hDragCursor;
	HCURSOR					m_hDragCursorAdd;
	CString					m_strData;			// spk의 소스가 되는 bmp의 위치 data
	CString					m_strShadowData;
	CString					m_strAniData;		// animation frame data
	CString					m_strOffsetData;	// creature의 출력 offset저장...
	CString					m_strCopyFlagData;	// animation frame이 복사된 것인가를 나타냄..

	BOOL					m_bDirty;			// save 해야 되나요?
	CAniDlg					m_aniDlg;

	CString					m_strDataFileName;
	BOOL					m_bCopied;			// copy&paste에서 copy된 object가 있나?
	int						m_iCopiedIndex;		// 복사된 index
	std::vector<CString>	m_szCopyData;









//	LPITEMIDLIST			m_pItemIDList;	// directory기억을 위한..
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.










#endif // !defined(AFX_SPRITEMANAGERDLG_H__90E8B5C8_45C9_11D4_AC04_0050DABAC3D4__INCLUDED_)

