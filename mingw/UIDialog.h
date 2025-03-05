//-----------------------------------------------------------------------------
// UIDialog.h
//-----------------------------------------------------------------------------
// Client에서 사용하는 UI Dilaog 모음..
//-----------------------------------------------------------------------------

#ifndef __UIDIALOG_H__
#define __UIDIALOG_H__

#include "DebugInfo.h"
class C_VS_UI_DIALOG;
class C_VS_UI_NPC_DIALOG;

#define UI_DIALOG_OK							0x0001

#define	FLAG_UIDIALOG_LOCK_PCTALK		0x01
#define	FLAG_UIDIALOG_LOCK_MESSAGE		0x02
//#define	FLAG_UIDIALOG_LOCK_HELP			0x04

class UIDialog {
	public :
		enum UIDIALOG_MESSAGE 
		{
			/*
			MESSAGE_CANNOT_BUY_NO_SPACE,
			MESSAGE_CANNOT_BUY_NO_MONEY,
			MESSAGE_CANNOT_BUY_NO_ITEM,
			MESSAGE_CANNOT_SELL,
			MESSAGE_SKILL_DIFFER_DOMAIN,
			MESSAGE_SKILL_EXCEED_LEVEL,
			MESSAGE_SKILL_CANNOT_LEARN,
			*/
			//MESSAGE_HELP,
			MESSAGE_PCTALK,
			MESSAGE_FREE,

			MAX_MESSAGE
		};

	public :
		UIDialog();
		~UIDialog();

		//---------------------------------------------------------
		// Release
		//---------------------------------------------------------
		void			Init();
		void			Release();

		//---------------------------------------------------------
		// Lock Input
		//---------------------------------------------------------
		static void		SetLockInputPCTalk()		{ s_LockGameInput |= FLAG_UIDIALOG_LOCK_PCTALK; }
		static void		SetLockInputMessage()		{ s_LockGameInput |= FLAG_UIDIALOG_LOCK_MESSAGE; }
		static void		UnSetLockInputPCTalk()		{ s_LockGameInput &= ~FLAG_UIDIALOG_LOCK_PCTALK; }
		static void		UnSetLockInputMessage()		{ s_LockGameInput &= ~FLAG_UIDIALOG_LOCK_MESSAGE; }
		static DWORD	IsLockInput()				{ return s_LockGameInput; }			

		//---------------------------------------------------------
		// PC Talk Dlg
		//---------------------------------------------------------
		void			PopupPCTalkDlg(int x=-1, int y=-1);		// -1은 center를 의미한다.(default)
		void			HidePCTalkDlg();
		void			ShowPCTalkDlg();
		static void		ProcessPCTalkDlg(C_VS_UI_DIALOG * pDlg, unsigned long id);
		void			ClosePCTalkDlg();

		//---------------------------------------------------------
		// Message Dlg
		//---------------------------------------------------------
		//void			PopupMessageDlg(UIDIALOG_MESSAGE msg, int x=-1, int y=-1);		// -1은 center를 의미한다.(default)		
		void			PopupFreeMessageDlg(const char* msg, int x=-1, int y=-1, WORD fButton = UI_DIALOG_OK, bool IsAfterExit = false);		// -1은 center를 의미한다.(default)				
		static void		ProcessMessageDlg(C_VS_UI_DIALOG * pDlg, unsigned long id);
		void			CloseMessageDlg();

		//---------------------------------------------------------
		// Message Dlg
		//---------------------------------------------------------
		//void			PopupHelpDlg(int x=-1, int y=-1);		// -1은 center를 의미한다.(default)		
		//static void		ProcessHelpDlg(C_VS_UI_DIALOG * pDlg, unsigned long id);
		//void			CloseHelpDlg();

	protected :
		static DWORD	s_LockGameInput;	// client로의 입력을 막는다.

		// PC TalkBox에 있는 말들 중에서 하나를 선택하게 하는 것..
		C_VS_UI_NPC_DIALOG*			m_pPCTalkDlg;
		//C_VS_UI_DIALOG*			m_pHelpDlg;
		C_VS_UI_DIALOG*			m_pMessageDlg;


		int 			m_ppDlgMessageSize[MAX_MESSAGE];
		char **			m_ppDlgMessage[MAX_MESSAGE];



#ifdef OUTPUT_DEBUG
		//---------------------------------------------------------
		// Test Client용
		//---------------------------------------------------------
	public :

		void				PopupAddonSelectDlg(int x=-1, int y=-1);		// -1은 center를 의미한다.(default)
		static void			ProcessAddonSelectDlg(C_VS_UI_DIALOG * pDlg, unsigned long id);

		void				PopupActionSelectDlg(int x=-1, int y=-1);		// -1은 center를 의미한다.(default)
		static void			ProcessActionSelectDlg(C_VS_UI_DIALOG * pDlg, unsigned long id);

		void				PopupDirectionSelectDlg(int x=-1, int y=-1);		// -1은 center를 의미한다.(default)
		static void			ProcessDirectionSelectDlg(C_VS_UI_DIALOG * pDlg, unsigned long id);

	protected :
		C_VS_UI_DIALOG*		m_pAddonSelectDlg;
		C_VS_UI_DIALOG*		m_pActionSelectDlg;
		C_VS_UI_DIALOG*		m_pDirectionSelectDlg;

#endif
	
};

extern UIDialog*			g_pUIDialog;

#endif

