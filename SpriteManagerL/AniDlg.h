#if !defined(AFX_ANIDLG_H__1543B100_4E8D_11D4_AC04_0050DABAC3D4__INCLUDED_)
#define AFX_ANIDLG_H__1543B100_4E8D_11D4_AC04_0050DABAC3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AniDlg.h : header file
//


#include <mmsystem.h>


//#include "SpriteManagerDlg.h"
#include "DIBList.h"

class CDIBList;


/////////////////////////////////////////////////////////////////
// From CLIENT
/////////////////////////////////////////////////////////////////
// 복장에 대한 값
enum ADDON
{
	ADDON_HAIR,			// 머리
	ADDON_HELM,			// 모자
	ADDON_COAT,			// 상의
	ADDON_TROUSER,		// 하의
	//ADDON_SHOES,		// 신발
	//ADDON_LEFTGLOVE,		// 왼손 장갑
	//ADDON_RIGHTGLOVE,		// 오른손 장갑
	ADDON_LEFTHAND,		// 왼손			
	ADDON_RIGHTHAND,	// 오른손
	//ADDON_CLOAK,		// 외투
	ADDON_MOTOR,		// 오토바이
	ADDON_MAX,
	ADDON_NULL	= 0xFFFF,
};

enum AC_ADDON
{
	ADDON_AC_HELMET,
	ADDON_AC_BODY,
	ADDON_AC_SHOULDER,
	ADDON_AC_LEFTHAND,
	ADDON_AC_RIGHTHAND,
	ADDON_AC_MOTOR,
	ADDON_AC_MAX
};


class CSpriteManagerDlg;


/////////////////////////////////////////////////////////////////////////////
// CAniDlg dialog

class CAniDlg : public CDialog
{
// Construction
public:
	void DrawScene();
	void CheckTime();
	void MakeAnimations();
	void ChangeBg();
	void ResetCombos();
	void UpdateInfoStatic();
//	CSpriteManagerDlg* GetParent();
	CAniDlg(CWnd* pParent = NULL);   // standard constructor
	~CAniDlg();

// Dialog Data
	//{{AFX_DATA(CAniDlg)
	enum { IDD = IDD_DIALOG_ANI };
	CComboBox	m_comboList[ADDON_MAX];
	CComboBox	m_comboVampire;
	CComboBox	m_comboOusters;
	CComboBox	m_comboOustersWeapon;
	CComboBox	m_comboACVampireWeapon;

	CComboBox	m_ACComboList[ ADDON_AC_MAX ];
	/*CComboBox	m_comboACHelmet;
	CComboBox	m_comboACLefthand;
	CComboBox	m_comboACRighthand;
	CComboBox	m_comboACShoulder;
	CComboBox	m_comboACMotor;*/

	CEdit		m_editFPS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAniDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	public:
	BOOL Create( UINT nIDTemplate, CWnd* pParentWnd = NULL );
	//}}AFX_VIRTUAL

	BOOL					m_bResetFPS;
	BOOL					m_bShowWindow;
	DWORD					m_dwUpdates;
	DWORD					m_dwOldTickCount;

	float					m_fRealFPS;
	int						m_iFPS;
	int						m_addons[ ADDON_MAX];	// part 정보.
	int						m_ACAddons[ ADDON_AC_MAX ];
	CDIBList				m_dibList[ ADDON_MAX];
	CDIBList				m_shadowDibList[ ADDON_MAX];
	BOOL					m_bPlayingAni;
	int						m_iAniTime;
	CSpriteManagerDlg*		m_pParentWnd;
//	CDib					m_bgDib;
	CBitmap					m_bgBitmap[4+1];	// +1 custom bmp...
	DWORD					m_dwBGBMP;
	CDC						m_backDC;
	CDC*					m_pMemDC;
	HBITMAP					m_pHOldBmps[100];	// 최대 100개..m_pMemDC에의해 선택된 bitmap을
	int						m_iCurAnis;		// 단순히 m_pMemDC의 bitmap을 제거하기 위한 기억
	HBITMAP					m_hOldBMP, m_hBMP;	// m_backDC를 위한 HBITMAP

	RECT					m_aniRect;

	float					m_fWidthRatio, m_fHeightRatio;	// animation 크기가 게임과 동일하도록 만들기 위한
															// 변수 원래 크기에 이를 곱한다.
	BOOL					m_bResize;	// animation출력시 resizing을 할 것인가?

	BOOL					m_bDragging;
	BOOL					m_bDrawTile;

	int						m_iCreatureXOffset, m_iCreatureYOffset;
	CPoint					m_fromPt, m_curPt;

	MMRESULT				m_timerID;	// multimedia timer id


	void Load( BOOL bForce = FALSE);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAniDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();

	afx_msg void OnSelchangeCombo();
	afx_msg void OnSelchangeComboBG();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonBg();
	afx_msg void OnCheckResize();
	afx_msg void OnCheckTile();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point) ;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point) ;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point) ;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point) ;
	afx_msg void OnChangeEditFPS();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus) ;
	afx_msg void OnClose();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIDLG_H__1543B100_4E8D_11D4_AC04_0050DABAC3D4__INCLUDED_)
