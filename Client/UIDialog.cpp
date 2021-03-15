//-----------------------------------------------------------------------------
// UIDialog.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "Client.h"			// 이거 안 넣으니까 ambigious.. T_T;;
#include "UIDialog.h"
#include "VS_UI_ExtraDialog.h" 
#include "TalkBox.h"
#include "PacketDef.h"
#include "MStringArray.h"
#include "TempInformation.h"
#include "VS_UI.h"
#include "ServerInfo.h"
#include "DebugInfo.h"
#include "UIFunction.h"
//#include <string>

#include "Packet/CPackets/CGSelectQuest.h"
#include "Packet/CPackets/CGSelectBloodBible.h"

#ifdef OUTPUT_DEBUG
	#include "MZone.h"
	#include "MTopView.h"
	#include "AddonDef.h"
#endif

bool g_bPartyRunning = false;		// ㅋㅋ...

using namespace std;

//-----------------------------------------------------------------------------
// global
//-----------------------------------------------------------------------------
UIDialog*			g_pUIDialog = NULL;

//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
DWORD				UIDialog::s_LockGameInput	= 0;

//-----------------------------------------------------------------------------
//
// constructor / destructor
//
//-----------------------------------------------------------------------------
UIDialog::UIDialog()
{
	m_pPCTalkDlg = NULL;

	m_pMessageDlg = NULL;

	//m_pHelpDlg = NULL;

	for (int i=0; i<MAX_MESSAGE; i++)
	{
		m_ppDlgMessage[i] = NULL;
	}

#ifdef OUTPUT_DEBUG
	m_pAddonSelectDlg = NULL;
	m_pActionSelectDlg = NULL;
	m_pDirectionSelectDlg = NULL;
#endif
	
}

UIDialog::~UIDialog()
{
	Release();
}

//-----------------------------------------------------------------------------
//
//	member functions
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
void
UIDialog::Init()
{
/*
	char* str = new char[128];

	//-------------------------------------------------------------
	// MESSAGE_CANNOT_BUY_NO_SPACE
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_CANNOT_BUY_NO_SPACE] = 1;	
	strcpy(str, g_GameStringTable[STRING_MESSAGE_CANNOT_BUY_NO_SPACE].GetString());
	m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_SPACE] = new char* [1];
	m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_SPACE][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_SPACE][0], str);

	//-------------------------------------------------------------
	// MESSAGE_CANNOT_BUY_NO_MONEY
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_CANNOT_BUY_NO_MONEY] = 1;
	strcpy(str, g_GameStringTable[STRING_MESSAGE_CANNOT_BUY_NO_MONEY].GetString());
	m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_MONEY] = new char* [1];
	m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_MONEY][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_MONEY][0], str);
			
	//-------------------------------------------------------------
	// MESSAGE_CANNOT_BUY_NO_ITEM
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_CANNOT_BUY_NO_ITEM] = 1;	
	strcpy(str, g_GameStringTable[STRING_MESSAGE_CANNOT_BUY_NO_ITEM].GetString());
	m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_ITEM] = new char* [1];
	m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_ITEM][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_CANNOT_BUY_NO_ITEM][0], str);

	//-------------------------------------------------------------
	// MESSAGE_CANNOT_BUY_NO_ITEM
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_CANNOT_SELL] = 1;	
	strcpy(str, g_GameStringTable[STRING_MESSAGE_CANNOT_SELL].GetString());
	m_ppDlgMessage[MESSAGE_CANNOT_SELL] = new char* [1];
	m_ppDlgMessage[MESSAGE_CANNOT_SELL][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_CANNOT_SELL][0], str);

	//-------------------------------------------------------------
	// MESSAGE_SKILL_DIFFER_DOMAIN
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_SKILL_DIFFER_DOMAIN] = 1;	
	strcpy(str, g_GameStringTable[STRING_MESSAGE_SKILL_DIFFER_DOMAIN].GetString());
	m_ppDlgMessage[MESSAGE_SKILL_DIFFER_DOMAIN] = new char* [1];
	m_ppDlgMessage[MESSAGE_SKILL_DIFFER_DOMAIN][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_SKILL_DIFFER_DOMAIN][0], str);

	//-------------------------------------------------------------
	// MESSAGE_SKILL_EXCEED_LEVEL
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_SKILL_EXCEED_LEVEL] = 1;	
	strcpy(str, g_GameStringTable[STRING_MESSAGE_SKILL_EXCEED_LEVEL].GetString());
	m_ppDlgMessage[MESSAGE_SKILL_EXCEED_LEVEL] = new char* [1];
	m_ppDlgMessage[MESSAGE_SKILL_EXCEED_LEVEL][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_SKILL_EXCEED_LEVEL][0], str);

	//-------------------------------------------------------------
	// MESSAGE_SKILL_CANNOT_LEARN
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_SKILL_CANNOT_LEARN] = 1;	
	strcpy(str, g_GameStringTable[STRING_MESSAGE_SKILL_CANNOT_LEARN].GetString());
	m_ppDlgMessage[MESSAGE_SKILL_CANNOT_LEARN] = new char* [1];
	m_ppDlgMessage[MESSAGE_SKILL_CANNOT_LEARN][0] = new char [strlen(str)+1];
	strcpy(m_ppDlgMessage[MESSAGE_SKILL_CANNOT_LEARN][0], str);

  	delete [] str;
*/

	//-------------------------------------------------------------
	// MESSAGE_PCTALK
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_PCTALK] = 1;	
	m_ppDlgMessage[MESSAGE_PCTALK] = new char* [1];
	for (int i=0; i<m_ppDlgMessageSize[MESSAGE_PCTALK]; i++)
	{
		m_ppDlgMessage[MESSAGE_PCTALK][i] = new char [2048];
	}
	


	//-------------------------------------------------------------
	// MESSAGE_FREE
	//-------------------------------------------------------------
	m_ppDlgMessageSize[MESSAGE_FREE] = 2;	
	m_ppDlgMessage[MESSAGE_FREE] = new char* [2];
	m_ppDlgMessage[MESSAGE_FREE][0] = new char [256];	
	m_ppDlgMessage[MESSAGE_FREE][1] = new char [256];	
}

//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void
UIDialog::Release()
{
	// PC Talk Dlg
	if (m_pPCTalkDlg!=NULL)
	{
		delete m_pPCTalkDlg;
		m_pPCTalkDlg = NULL;
	}

	// Cannot buy Dlg
	if (m_pMessageDlg!=NULL)
	{
		delete m_pMessageDlg;
		m_pMessageDlg = NULL;
	}

	// help
	/*
	if (m_pHelpDlg!=NULL)
	{
		delete m_pHelpDlg;
		m_pHelpDlg = NULL;
	}
	*/

	// dlg message
	for (int i=0; i<MAX_MESSAGE; i++)
	{
		if (m_ppDlgMessage[i]!=NULL)
		{
			for (int j=0; j<m_ppDlgMessageSize[i]; j++)
			{
				if (m_ppDlgMessage[i][j]!=NULL)
				{
					delete [] m_ppDlgMessage[i][j];
				}
			}

			delete [] m_ppDlgMessage[i];

			m_ppDlgMessage[i] = NULL;
		}
	}	
	

	// 입력 제한 해제
	s_LockGameInput	= 0;	

#ifdef OUTPUT_DEBUG
	if (m_pAddonSelectDlg!=NULL)
	{
		delete m_pAddonSelectDlg;
		m_pAddonSelectDlg = NULL;
	}

	if (m_pActionSelectDlg!=NULL)
	{
		delete m_pActionSelectDlg;
		m_pActionSelectDlg = NULL;
	}

	if (m_pDirectionSelectDlg!=NULL)
	{
		delete m_pDirectionSelectDlg;
		m_pDirectionSelectDlg = NULL;
	}
#endif
}

//-----------------------------------------------------------------------------
// Close PCTalk Dlg
//-----------------------------------------------------------------------------
void			
UIDialog::ClosePCTalkDlg()
{
	if (m_pPCTalkDlg!=NULL)
	{
		delete m_pPCTalkDlg;
		m_pPCTalkDlg = NULL;

		UnSetLockInputPCTalk();

		// 파티가 떠 있었다면... 다시 띄운다.
		if (g_bPartyRunning)
		{
			UI_RunParty();
			g_bPartyRunning = false;
		}
	}
}

//-----------------------------------------------------------------------------
// Close FreeMessage Dlg
//-----------------------------------------------------------------------------
void			
UIDialog::CloseMessageDlg()
{
	if (m_pMessageDlg!=NULL)
	{
		delete m_pMessageDlg;
		m_pMessageDlg = NULL;

		UnSetLockInputMessage();
	}
}

//-----------------------------------------------------------------------------
// Close Help Dlg
//-----------------------------------------------------------------------------
/*
void			
UIDialog::CloseHelpDlg()
{
	if (m_pHelpDlg!=NULL)
	{
		delete m_pHelpDlg;
		m_pHelpDlg = NULL;		
	}
}
*/

//-----------------------------------------------------------------------------
// Hide PCTalk Dlg
//-----------------------------------------------------------------------------
void			
UIDialog::HidePCTalkDlg()
{
	if (m_pPCTalkDlg!=NULL)
	{
		m_pPCTalkDlg->Finish();
	}
}

//-----------------------------------------------------------------------------
// Show PCTalk Dlg
//-----------------------------------------------------------------------------
void			
UIDialog::ShowPCTalkDlg()
{
	if (m_pPCTalkDlg!=NULL)
	{
		m_pPCTalkDlg->Start();		
	}
}

//-----------------------------------------------------------------------------
// NPC 대화 선택 dialog
//-----------------------------------------------------------------------------
void 
UIDialog::ProcessPCTalkDlg(C_VS_UI_DIALOG * pDlg, id_t id)
{
	//-----------------------------------------------------------
	// 뭔가를 선택했을 때, 
	// server로 선택된 것을 보낸다.
	//-----------------------------------------------------------
	// 삽질 코드.. - -;
	g_pPCTalkBox->SetAnswerID(id);

	int answerID;
	//--------------------------------------------------
	// dialog를 '끝'냈을 때..
	//--------------------------------------------------
	//if (id==DIALOG_EXECID_EXIT)
	//{
	//	answerID = 99;		// - -;;

		//UnSetLockInputPCTalk();		
	//}
	//--------------------------------------------------
	// 정상적인 선택..
	//--------------------------------------------------
	//else
	{
		answerID = g_pPCTalkBox->GetAnswerID();
	}
	

	//--------------------------------------------------
	// 검증 받을게 없는 경우에만 packet을 보낸다.
	//--------------------------------------------------
	if (g_Mode == MODE_GAME)
	{
		if (g_pPlayer->IsWaitVerifyNULL())
		{	
			//--------------------------------------------------
			// 다른 UI가 떠있지 않는 경우에..
			//--------------------------------------------------
			if (!UI_IsRunningStorage() 
				&& !UI_IsRunningExchange()
				&& !UI_IsRunningShop()
				&& !UI_IsRunningBookcase()
				&& !UI_IsRunningRepair()
				&& !UI_IsRunningSell()
				&& !UI_IsRunningSwapAdvancementItem())
			{
				switch (g_pPCTalkBox->GetType())
				{				
					//--------------------------------------------------
					// 일반적인 대화
					//--------------------------------------------------
					case PCTalkBox::NORMAL :
					{						
						if( g_pPCTalkBox->m_AnswerIDMap.size() >= answerID )
							answerID = g_pPCTalkBox->m_AnswerIDMap[answerID-1] + 1;
						
						CGNPCAskAnswer _CGNPCAskAnswer;

						_CGNPCAskAnswer.setObjectID( g_pPCTalkBox->GetNPCID() );
						_CGNPCAskAnswer.setScriptID( g_pPCTalkBox->GetScriptID() );
						_CGNPCAskAnswer.setAnswerID( answerID );
						
						g_pSocket->sendPacket( &_CGNPCAskAnswer );

						//--------------------------------------------------
						// 검증 packet을 기다리는 mode로 설정한다.
						//--------------------------------------------------
						g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NPC_ASK );						
					}
					break;

					//--------------------------------------------------
					// 기술 배울 떄
					//--------------------------------------------------
					case PCTalkBox::SKILL_LEARN :
					{
						if (id!=DIALOG_EXECID_EXIT)
						{
							if (id==999)
							{
								// 빠져 나가기
								pDlg->Run( DIALOG_EXECID_EXIT );

								UnSetLockInputPCTalk();

								g_pPlayer->SetWaitVerifyNULL();

								g_pTempInformation->SetMode(TempInformation::MODE_NULL);
							}
							else if (g_pTempInformation->GetMode()==TempInformation::MODE_SKILL_LEARN)
							{
								g_pTempInformation->SetMode(TempInformation::MODE_NULL);

								CGLearnSkill _CGLearnSkill;
					
								//_CGLearnSkill.setObjectID( (*g_pPCTalkBox).GetNPCID() );
								_CGLearnSkill.setSkillType( id );
								_CGLearnSkill.setSkillDomainType( (*g_pTempInformation).Value1 );

								g_pSocket->sendPacket( &_CGLearnSkill );

								//--------------------------------------------------
								// 검증 packet을 기다리는 mode로 설정한다.
								//--------------------------------------------------
								//g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NPC_ASK );						
								
								// Dialog 빠져 나가기
								pDlg->Run( DIALOG_EXECID_EXIT );

								g_pPlayer->SetWaitVerifyNULL();								

								UnSetLockInputPCTalk();

								g_pTempInformation->SetMode(TempInformation::MODE_NULL);
							}
							else
							{
								DEBUG_ADD("[Error] Mode is Not SKILL_LEARN");								
							}
						}
					}
					break;
					case PCTalkBox::SELECT_QUEST :
					{
						if ( id != DIALOG_EXECID_EXIT )
						{
							if( id == 9999 )
							{
								pDlg->Run( DIALOG_EXECID_EXIT );
								UnSetLockInputPCTalk();
								g_pPlayer->SetWaitVerifyNULL();
								g_pTempInformation->SetMode(TempInformation::MODE_NULL);
							} else if (g_pTempInformation->GetMode() == TempInformation::MODE_SELECT_QUEST )
							{								
								CGSelectQuest _CGSelectQuest;

								_CGSelectQuest.setQuestID ( id );
								_CGSelectQuest.setNPCObjectID( g_pPCTalkBox->GetNPCID() );
								g_pSocket->sendPacket( &_CGSelectQuest );
								
								pDlg->Run( DIALOG_EXECID_EXIT );
								g_pPlayer->SetWaitVerifyNULL();
								UnSetLockInputPCTalk();								
								g_pTempInformation->SetMode( TempInformation::MODE_NULL );
							} else
							{
								DEBUG_ADD("[Error] Mode is Not SELECT_QUEST");
							}
						}
					}
					break;
					//--------------------------------------------------
					// 블러디 바이블  배울 떄
					//--------------------------------------------------
					case PCTalkBox::BLOOD_BIBLE_SIGN :
					{
						if (id!=DIALOG_EXECID_EXIT)
						{
							if (id==999)
							{
								// 빠져 나가기
								pDlg->Run( DIALOG_EXECID_EXIT );

								UnSetLockInputPCTalk();

								g_pPlayer->SetWaitVerifyNULL();

								g_pTempInformation->SetMode(TempInformation::MODE_NULL);
							}
							else if (g_pTempInformation->GetMode()==TempInformation::MODE_SKILL_LEARN)
							{
							//	g_pTempInformation->SetMode(TempInformation::MODE_NULL);

								CGSelectBloodBible _CGSelectBloodBible;
					
								//_CGLearnSkill.setObjectID( (*g_pPCTalkBox).GetNPCID() );
								_CGSelectBloodBible.setBloodBibleID( id );
								
								g_pSocket->sendPacket( &_CGSelectBloodBible );

								//--------------------------------------------------
								// 검증 packet을 기다리는 mode로 설정한다.
								//--------------------------------------------------
								//g_pPlayer->SetWaitVerify( MPlayer::WAIT_VERIFY_NPC_ASK );						
								
								// Dialog 빠져 나가기
//								pDlg->Run( DIALOG_EXECID_EXIT );
//
//								g_pPlayer->SetWaitVerifyNULL();								
//
//								UnSetLockInputPCTalk();
//
//								g_pTempInformation->SetMode(TempInformation::MODE_NULL);
							}
							else
							{
								DEBUG_ADD("[Error] Mode is Not SKILL_LEARN");								
							}
						}
					}
					break;
				}
			}			
		}
	}

	//-------------------------------------------------------------
	// ESC 누르거나.. 그냥 빠질때.. 검증... 으흠.. 뭔가 불안.. - -;
	//-------------------------------------------------------------
	if (id==DIALOG_EXECID_EXIT)
	{
		UnSetLockInputPCTalk();

		g_pPlayer->SetWaitVerifyNULL();

		g_pTempInformation->SetMode(TempInformation::MODE_NULL);

		// 파티가 떠 있었다면... 다시 띄운다.
		if (g_bPartyRunning)
		{
			UI_RunParty();
			g_bPartyRunning = false;
		}
	}
}

//-----------------------------------------------------------------------------
// PopupPCTalkDlg
//-----------------------------------------------------------------------------
void
UIDialog::PopupPCTalkDlg(int x, int y)
{
//				C_VS_UI_DIALOG m_pC_dialog = new C_VS_UI_DIALOG(50, 20, 6, 2, func, DIALOG_OK);
//
//				DIALOG_MENU d_menu[] = {
//					{"사기, 0},
//					{"팔기", 1},
//					{"끝내기", DIALOG_EXECID_EXIT},
//				};
//				m_pC_dialog->SetMenu(d_menu, 3);
//
//				static char * pp_dmsg[] = { // Message는 반드시 static or global로 해야 한다.
//					"line 1",
//					"line 2",
//				};
//
//				m_pC_dialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *))

	extern bool	g_bTestMode;

	if(g_bActiveGame == false
#ifdef OUTPUT_DEBUG
		&& !g_bTestMode
#endif
		)
		return;
	
	//---------------------------------------------------------
	// 기존에 있던 dialog를 지운다.
	//---------------------------------------------------------
	if (m_pPCTalkDlg!=NULL)
	{
		delete m_pPCTalkDlg;		
		m_pPCTalkDlg = NULL;
	}
	
	// 파티가 떠 있었는지 확인
	if (UI_IsRunningParty() || g_bPartyRunning)
	{
		g_bPartyRunning = true;
	}
	else
	{
		g_bPartyRunning = false;
	}

	//---------------------------------------------------------
	// 다른 dialog들 닫기
	//---------------------------------------------------------
	DEBUG_ADD("UID-CloseUIDlg");

//	gC_vs_ui.CloseInfo();
//	gC_vs_ui.CloseShop();
//	gC_vs_ui.ServerDisconnectMessage();
	gC_vs_ui.CloseAllDialog();

	DEBUG_ADD("UID_COK");

	CloseMessageDlg();
	
	//---------------------------------------------------------
	// size 체크
	//---------------------------------------------------------
	int msgSize = g_pPCTalkBox->size();

	if (msgSize==0)
	{
		return;
	}

	//---------------------------------------------------------
	// 제목
	//---------------------------------------------------------
	const char*	content = g_pPCTalkBox->GetContent();
	int lenContent = strlen(content);

	//---------------------------------------------------------
	// dialog의 길이를 정한다.
	//---------------------------------------------------------
	// -1 넣으면 자동이다.
	/*
	int lengthY;
	
	if (g_pPCTalkBox->size() < 2)
	{
		lengthY = 0;
	}
	else
	{
		lengthY = g_pPCTalkBox->size() / 3;
	}

	lengthY += lenContent/150 + 1;	//(lenContent/40)/3 + 1;
	*/
	//---------------------------------------------------------
	// dialog 생성
	//---------------------------------------------------------
	DEBUG_ADD("newNPCDLG");

	m_pPCTalkDlg = new C_VS_UI_NPC_DIALOG(ProcessPCTalkDlg,
							g_pPCTalkBox->GetCreatureType(), 
							(*g_pCreatureTable)[g_pPCTalkBox->GetCreatureType()].Name.GetString());
							//g_pZone->GetCreature(g_pPCTalkBox->GetNPCID())->GetName());//, SMO_NOFIT);

	//---------------------------------------------------------
	// 제목 등록
	//---------------------------------------------------------	
	/*
	char* pContent = new char [lenContent+1];
	strcpy( pContent, content );
	char* pContentTemp = pContent;

	//-------------------------------------------------------------
	// 40글자 이상
	//-------------------------------------------------------------
	// 콩가루~~~ ㅡ.ㅡ; 귀차나.. 음냐..
	int numStr = 0;
	
	while (lenContent > 40)
	{
		char* str = pContentTemp + 40;

		for (int i=40; i>0; i--)
		{
			if (*str==' ')
			{
				break;
			}

			str--;
		}

		strncpy(m_ppDlgMessage[MESSAGE_PCTALK][numStr], pContentTemp, i);
		m_ppDlgMessage[MESSAGE_PCTALK][numStr][i] = '\0';

		lenContent -= i+1;

		pContentTemp += i+1;		// +1은 공백이다.		

		numStr++;
	}

	//-------------------------------------------------------------
	// 40글자 이하
	//-------------------------------------------------------------
	strcpy(m_ppDlgMessage[MESSAGE_PCTALK][numStr], pContentTemp);
	numStr++;	

	m_pPCTalkDlg->SetMessage(m_ppDlgMessage[MESSAGE_PCTALK], numStr);//sizeof(pp_dmsg)/sizeof(char *));
	*/


	//---------------------------------------------------------
	// 메뉴 구성..
	//---------------------------------------------------------
	DEBUG_ADD("newDLGMENU");
	DIALOG_MENU* pMenu = new DIALOG_MENU[msgSize];// + 1];

	MStringList::iterator iString = g_pPCTalkBox->begin();

	int i=0;

	switch (g_pPCTalkBox->GetType())
	{
		//-----------------------------------------------------
		// 일반적인 대화
		//-----------------------------------------------------
		case PCTalkBox::NORMAL :
			while (iString != g_pPCTalkBox->end())
			{
				MString* pString = *iString;

				// menu 내용 설정
				pMenu[i].exec_id		= i+1;						// ID+1
				pMenu[i].sz_menu_str	= pString->GetString();		// 내용..
				
				// 다음..
				iString++;
				i++;		
			}
		break;

		//-----------------------------------------------------
		// 기술 배울 때
		//-----------------------------------------------------
		// %3d%s 형식으로 되어 있다. 임시!~~~ 쩝~~
		//-----------------------------------------------------
		case PCTalkBox::SKILL_LEARN :
		case PCTalkBox::BLOOD_BIBLE_SIGN:
			while (iString != g_pPCTalkBox->end())
			{
				MString* pString = *iString;
				DEBUG_ADD(pString->GetString());

				char strID[80];
				char strName[80];

				strncpy(strID, pString->GetString(), 3);
				strID[3] = NULL;
				strcpy(strName, pString->GetString()+3);

				// 이름 다시 설정
				*pString = strName;
				
				// menu 내용 설정
				pMenu[i].exec_id		= atoi(strID);
				pMenu[i].sz_menu_str	= pString->GetString();		// 내용..
				
				// 다음..
				iString++;
				i++;		
			}
		break;
		case PCTalkBox::SELECT_QUEST :
			while (iString != g_pPCTalkBox->end())
			{
				MString* pString = *iString;
				DEBUG_ADD(pString->GetString());

				char strID[80];
				char strName[80];

				strncpy(strID, pString->GetString(), 4);
				strID[4] = NULL;
				strcpy(strName, pString->GetString()+4);

				// 이름 다시 설정
				*pString = strName;
				
				// menu 내용 설정
				pMenu[i].exec_id		= atoi(strID);
				pMenu[i].sz_menu_str	= pString->GetString();		// 내용..
				
				// 다음..
				iString++;
				i++;		
			}
			break;
	}


	// "끝내기"추가
	//pMenu[i].exec_id		= DIALOG_EXECID_EXIT;			// UI에서 정한 ID
	//pMenu[i].sz_menu_str	= new char [10];
	//strcpy(pMenu[i].sz_menu_str, "나가기");

	//---------------------------------------------------------
	// 메뉴 등록
	//---------------------------------------------------------
	DEBUG_ADD("setMENU");
	m_pPCTalkDlg->SetMenu(pMenu, msgSize, false);// + 1, false);		// 끝내기 포함
	
	DEBUG_ADD("spMenu");
	strcpy(m_ppDlgMessage[MESSAGE_PCTALK][0], g_pPCTalkBox->GetContent());

	//const char* pFirstString = m_ppDlgMessage[MESSAGE_PCTALK][0];

	DEBUG_ADD("setMsg");

	m_pPCTalkDlg->SetMessage(m_ppDlgMessage[MESSAGE_PCTALK], 1);//sizeof(pp_dmsg)/sizeof(char *));

	//---------------------------------------------------------
	// delete
	//---------------------------------------------------------
	//delete [] pMenu[i].sz_menu_str;		// 끝내기 string만 지워주면 된다.
	delete [] pMenu;
//	delete [] pContent;

	//---------------------------------------------------------
	// dialog 시작..
	//---------------------------------------------------------
	DEBUG_ADD("startTalk");
	m_pPCTalkDlg->Start();

	//---------------------------------------------------------
	// game으로의 입력을 차단한다.
	//---------------------------------------------------------
	SetLockInputPCTalk();
}

//-----------------------------------------------------------------------------
// Help dialog
//-----------------------------------------------------------------------------
/*
void 
UIDialog::ProcessHelpDlg(C_VS_UI_DIALOG * pDlg, id_t id)
{
	//--------------------------------------------------
	// dialog를 '끝'냈을 때..
	//--------------------------------------------------
	if (id==DIALOG_EXECID_EXIT)
	{
		//UnSetLockInputHelp();
	}
}
*/

//-----------------------------------------------------------------------------
// PopupHelpDlg
//-----------------------------------------------------------------------------
/*
void
UIDialog::PopupHelpDlg(int x, int y)
{
//				C_VS_UI_DIALOG m_pC_dialog = new C_VS_UI_DIALOG(50, 20, 6, 2, func, DIALOG_OK);
//
//				DIALOG_MENU d_menu[] = {
//					{"사기, 0},
//					{"팔기", 1},
//					{"끝내기", DIALOG_EXECID_EXIT},
//				};
//				m_pC_dialog->SetMenu(d_menu, 3);
//
//				static char * pp_dmsg[] = { // Message는 반드시 static or global로 해야 한다.
//					"line 1",
//					"line 2",
//				};
//
//				m_pC_dialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *))

	//---------------------------------------------------------
	// 기존에 있던 dialog를 지운다.
	//---------------------------------------------------------
	if (m_pHelpDlg!=NULL)
	{
		delete m_pHelpDlg;
	}


	//---------------------------------------------------------
	// dialog 생성
	//---------------------------------------------------------
	m_pHelpDlg = new C_VS_UI_DIALOG(x, y, 4, 3, ProcessHelpDlg, SMO_NOFIT);

	//---------------------------------------------------------
	// 도움말을 Loading한다.
	//---------------------------------------------------------
	if (m_ppDlgMessage[MESSAGE_HELP]==NULL)
	{	
		MStringArray	helpMessage;
		
		ifstream file(FILE_INFO_HELP, std::ios::binary);
		helpMessage.LoadFromFile( file );
		file.close();

		int lines = helpMessage.GetSize();

		m_ppDlgMessage[MESSAGE_HELP] = new char* [lines];

		for (int i=0; i<lines; i++)
		{
			m_ppDlgMessage[MESSAGE_HELP][i] = new char [helpMessage[i].GetLength()+1];
			strcpy(m_ppDlgMessage[MESSAGE_HELP][i], helpMessage[i].GetString());
		}

		m_ppDlgMessageSize[MESSAGE_HELP] = lines;	
	}

	m_pHelpDlg->SetMessage(m_ppDlgMessage[MESSAGE_HELP], m_ppDlgMessageSize[MESSAGE_HELP]);

	//---------------------------------------------------------
	// dialog 시작..
	//---------------------------------------------------------
	m_pHelpDlg->Start();

	//---------------------------------------------------------
	// game으로의 입력을 차단한다.
	//---------------------------------------------------------
	//SetLockInputHelp();
}
*/

//-----------------------------------------------------------------------------
// Popup MessageDlg
//-----------------------------------------------------------------------------
/*
void
UIDialog::PopupMessageDlg(UIDIALOG_MESSAGE msg, int x, int y)
{
//				C_VS_UI_DIALOG m_pC_dialog = new C_VS_UI_DIALOG(50, 20, 6, 2, func, DIALOG_OK);
//
//				DIALOG_MENU d_menu[] = {
//					{"사기, 0},
//					{"팔기", 1},
//					{"끝내기", DIALOG_EXECID_EXIT},
//				};
//				m_pC_dialog->SetMenu(d_menu, 3);
//
//				static char * pp_dmsg[] = { // Message는 반드시 static or global로 해야 한다.
//					"line 1",
//					"line 2",
//				};
//
//				m_pC_dialog->SetMessage(pp_dmsg, sizeof(pp_dmsg)/sizeof(char *))

	if (msg >= MAX_MESSAGE || msg==MESSAGE_FREE)
	{
		return;
	}

	//---------------------------------------------------------
	// 기존에 있던 dialog를 지운다.
	//---------------------------------------------------------
	if (m_pMessageDlg!=NULL)
	{
		delete m_pMessageDlg;
	}

	//---------------------------------------------------------
	// dialog 생성
	//---------------------------------------------------------
	m_pMessageDlg = new C_VS_UI_DIALOG(x, y, 3, 0, ProcessMessageDlg, DIALOG_OK);

	m_pMessageDlg->SetMessage(m_ppDlgMessage[msg], m_ppDlgMessageSize[msg], SMO_NOFIT);//sizeof(pp_dmsg)/sizeof(char *));


	//---------------------------------------------------------
	// dialog 시작..
	//---------------------------------------------------------
	m_pMessageDlg->Start();

	//---------------------------------------------------------
	// game으로의 입력을 차단한다.
	//---------------------------------------------------------
	SetLockInputHelp();
}
*/

//-----------------------------------------------------------------------------
// Popup Free MessageDlg
//-----------------------------------------------------------------------------
void
UIDialog::PopupFreeMessageDlg(const char* msg, int x, int y, WORD fButton, bool IsAfterExit)
{
	int msgID= MESSAGE_FREE;
	
	//-------------------------------------------------------------
	// MESSAGE_FREE
	//-------------------------------------------------------------
	int msgSize = strlen(msg);

	int dlgSizeY, numStr;

	//-------------------------------------------------------------
	// 40글자 이상
	//-------------------------------------------------------------
	// 콩가루~~~ ㅡ.ㅡ; 귀차나.. 음냐..
	if (msgSize > 40)
	{
		dlgSizeY = 1;
		char* str = (char*)msg + 40;

		for (int i=40; i>0; i--)
		{
			if (*str==' ')
			{
				break;
			}

			str--;
		}

		strncpy(m_ppDlgMessage[msgID][0], msg, i);
		m_ppDlgMessage[msgID][0][i] = '\0';
		//------------------------------------------
		//yckou
//		strcpy(m_ppDlgMessage[msgID][1], str+1);
		if(i==0)
			strcpy(m_ppDlgMessage[msgID][1], str);
		else
			strcpy(m_ppDlgMessage[msgID][1], str+1);
		//end yckou
		//------------------------------------------
		numStr = 2;
	}
	//-------------------------------------------------------------
	// 40글자 이하
	//-------------------------------------------------------------
	else
	{
		dlgSizeY = 0;
		strcpy(m_ppDlgMessage[msgID][0], msg);
		numStr = 1;
	}

	//---------------------------------------------------------
	// 기존에 있던 dialog를 지운다.
	//---------------------------------------------------------
	if (m_pMessageDlg!=NULL)
	{
		delete m_pMessageDlg;
		m_pMessageDlg = NULL;
	}

	//---------------------------------------------------------
	// dialog 생성
	//---------------------------------------------------------
	m_pMessageDlg = new C_VS_UI_DIALOG(x, y, 3, dlgSizeY, ProcessMessageDlg, fButton);

	//DEBUG_ADD_FORMAT("UIDIALOG(%s)", m_ppDlgMessage[msgID][0]);
	m_pMessageDlg->SetMessage(m_ppDlgMessage[msgID], numStr, SMO_NOFIT);//sizeof(pp_dmsg)/sizeof(char *));
	//DEBUG_ADD_FORMAT("UIDIALOG(%s)after", m_ppDlgMessage[msgID][0]);


	//---------------------------------------------------------
	// dialog 시작..
	//---------------------------------------------------------
	m_pMessageDlg->Start();

	//---------------------------------------------------------
	// game으로의 입력을 차단한다.
	//---------------------------------------------------------
	SetLockInputMessage();
}
//-----------------------------------------------------------------------------
// ProcessCannotBuyDlg
//-----------------------------------------------------------------------------
void		
UIDialog::ProcessMessageDlg(C_VS_UI_DIALOG * pDlg, unsigned long id)
{
	//if (id==DIALOG_OK)
	//{
		UnSetLockInputMessage();
	//}

}



#ifdef OUTPUT_DEBUG
	//-----------------------------------------------------------------------------
	// Popup AddonSelect Dlg
	//-----------------------------------------------------------------------------
	void				
	UIDialog::PopupAddonSelectDlg(int x, int y)
	{
		//---------------------------------------------------------
		// 기존에 있던 dialog를 지운다.
		//---------------------------------------------------------
		if (m_pAddonSelectDlg!=NULL)
		{
			delete m_pAddonSelectDlg;
		}

	
		//---------------------------------------------------------
		// dialog 생성
		//---------------------------------------------------------
		m_pAddonSelectDlg = new C_VS_UI_DIALOG(x, y, 1, 2, ProcessAddonSelectDlg);//, SMO_NOFIT);

		//---------------------------------------------------------
		// 메뉴 구성..
		//---------------------------------------------------------
		const int menuSize = 9;
		DIALOG_MENU menu[ menuSize ] = {
					{"머리", 1},
					{"모자", 2},
					{"상의", 3},
					{"하의", 4},
					{"근접무기", 5},
					{"총", 6},
					{"방패", 7},
					{"오토바이", 8},
					{"EXIT", DIALOG_EXECID_EXIT},					
				};

		//---------------------------------------------------------
		// 메뉴 등록
		//---------------------------------------------------------
		m_pAddonSelectDlg->SetMenu(menu, menuSize);		// 끝내기 포함

		//---------------------------------------------------------
		// dialog 시작..
		//---------------------------------------------------------
		m_pAddonSelectDlg->Start();
	}

	//-----------------------------------------------------------------------------
	// Process AddonSelect Dlg
	//-----------------------------------------------------------------------------
	void			
	UIDialog::ProcessAddonSelectDlg(C_VS_UI_DIALOG * pDlg, unsigned long id)
	{
		switch (id)
		{
			case DIALOG_EXECID_EXIT :
			break;

			//----------------------------------------------------------------------
			// 머리
			//----------------------------------------------------------------------
			case 1 :
			{
				static int count = 1;

				if (count==0)
				{
					g_pPlayer->RemoveAddon(ADDON_HAIR);
				}
				else
				{
					const int hairFrame[] =
					{
						ADDONID_HAIR1_MALE,
						ADDONID_HAIR2_MALE,
						ADDONID_HAIR3_MALE
					};

					const int hairFrameFemale[] =
					{
						ADDONID_HAIR1_FEMALE,
						ADDONID_HAIR2_FEMALE,
						ADDONID_HAIR3_FEMALE
					};

					if (g_pPlayer->IsMale())
					{
						g_pPlayer->SetAddonHair(hairFrame[count-1], 0);
					}
					else
					{
						g_pPlayer->SetAddonHair(hairFrameFemale[count-1], 0);
					}
				}

				if (++count==4) count = 0;
			}
			break;

			//----------------------------------------------------------------------
			// 모자
			//----------------------------------------------------------------------
			case 2 :
			{
				static int count = 1;

				if (count==0)
				{
					g_pPlayer->RemoveAddon(ADDON_HELM);
				}
				else
				{
					const int helmType[] =
					{
						0, 4
					};

					MItem*	pHelm = new MHelm;	pHelm->SetItemType( helmType[count-1] ); pHelm->ClearItemOption();
					g_pPlayer->SetAddonItem( pHelm );	
					delete pHelm;
				}

				if (++count==3) count = 0; 
			}
			break;

			//----------------------------------------------------------------------
			// 상의
			//----------------------------------------------------------------------
			case 3 :
			{
				static int count = 1;

				if (count==0)
				{
					g_pPlayer->RemoveAddon(ADDON_COAT);
				}
				else
				{
					const int coatType[] =
					{
						0, 4
					};

					MItem*	pCoat = new MCoat;	pCoat->SetItemType( coatType[count-1] ); pCoat->ClearItemOption();
					g_pPlayer->SetAddonItem( pCoat );	
					delete pCoat;
				}

				if (++count==3) count = 0; 
			}
			break;

			//----------------------------------------------------------------------
			// 하의
			//----------------------------------------------------------------------
			case 4 :
			{
				static int count = 1;

				if (count==0)
				{
					g_pPlayer->RemoveAddon(ADDON_TROUSER);
				}
				else
				{
					const int TrouserType[] =
					{
						0, 4
					};

					MItem*	pTrouser = new MTrouser;	pTrouser->SetItemType( TrouserType[count-1] ); pTrouser->ClearItemOption();
					g_pPlayer->SetAddonItem( pTrouser );	
					delete pTrouser;
				}

				if (++count==3) count = 0; 
			}
			break;

			//----------------------------------------------------------------------
			// 근접무기
			//----------------------------------------------------------------------
			case 5 :
			{
				static int count = 1;

				g_pPlayer->RemoveAddon( ADDON_RIGHTHAND );

				switch (count)
				{
					//-------------------------------------------------------
					// BLADE
					//-------------------------------------------------------
					case 1 :				
					{
						MItem*	pBlade = new MBlade;	pBlade->SetItemType( 0 ); pBlade->ClearItemOption();
						g_pPlayer->SetAddonItem( pBlade );	
						delete pBlade;
					}
					break;

					//-------------------------------------------------------
					// SWORD
					//-------------------------------------------------------
					case 2 :
					{
						MItem*	pSword = new MSword;	pSword->SetItemType( 0 );	pSword->ClearItemOption();
						g_pPlayer->SetAddonItem( pSword );
						delete pSword;
					}
					break;

					//-------------------------------------------------------
					// CROSS
					//-------------------------------------------------------
					case 3 :
					{
						MItem*	pCross = new MCross;	pCross->SetItemType( 0 );	pCross->ClearItemOption();
						g_pPlayer->SetAddonItem( pCross);
						delete pCross;
					}
					break;
				}

				if (++count == 4)	count = 0;
			
			}
			break;

			//----------------------------------------------------------------------
			// 총
			//----------------------------------------------------------------------
			case 6 :
			{
				static int count = 1;

				g_pPlayer->RemoveAddon( ADDON_RIGHTHAND );
				g_pPlayer->RemoveAddon( ADDON_LEFTHAND );
					
								
				MGunItem* pGunItem;
				
				if (count!=0)
				{
					switch (count)
					{
						case 1 :
							pGunItem = new MGunTR;
						break;

						case 2 :
							pGunItem = new MGunSMG;
						break;

						case 3 :
							pGunItem = new MGunAR;
						break;

						case 4 :
							pGunItem = new MGunSG;
						break;						
					}

					pGunItem->SetItemType( 0 ); pGunItem->ClearItemOption();				
				
					MMagazine* pMagazine = (MMagazine*)MItem::NewItem( (ITEM_CLASS)ITEM_CLASS_MAGAZINE );

					// 의미 없음 - -;
					pMagazine->SetID( 0 );

					// 이거는 총에 맞춰서 해줘야된다.
					for (int j=0; j<(*g_pItemTable)[ITEM_CLASS_MAGAZINE].GetSize(); j++)			
					{
						pMagazine->SetItemType(	j );

						if (pMagazine->IsInsertToItem( pGunItem ))
						{
							break;
						}
					}

					// 의미 없음
					pMagazine->ClearItemOption();

					// 탄창 개수
					pMagazine->SetNumber( 0xFFFF );

					//------------------------------------
					// 탄창 설정
					//------------------------------------
					pGunItem->SetMagazine( pMagazine );

					g_pPlayer->SetAddonItem( pGunItem );

					delete pGunItem;
				}				

				
				if (++count==5) count = 0;
			}
			break;


			//----------------------------------------------------------------------
			// 방패
			//----------------------------------------------------------------------
			case 7 :	
			{
				static int count = 1;

				if (count==0)
				{
					g_pPlayer->RemoveAddon(ADDON_LEFTHAND);
				}
				else
				{				
					const int itemType[] = { 0, 5 };
					MItem*	pShield = new MShield;	pShield->SetItemType( itemType[count-1] ); pShield->ClearItemOption();

					g_pPlayer->RemoveAddon( ADDON_LEFTHAND );

					g_pPlayer->SetAddonItem( pShield );		

					delete pShield;				
				}

				
				if (++count==3) count = 0;
			}
			break;

			//----------------------------------------------------------------------
			// 오토바이
			//----------------------------------------------------------------------
			case 8 :
			{
				static int count = 1;

				if (count==0)
				{
					g_pPlayer->RemoveAddon(ADDON_MOTOR);
				}
				else
				{
					MItem*	pMotor = new MMotorcycle;	pMotor->SetItemType( 0 ); pMotor->ClearItemOption();
					g_pPlayer->SetAddonItem( pMotor );	
					delete pMotor;
				}

				if (++count==2) count = 0; 
			}
			break;
		}
	}

	//-----------------------------------------------------------------------------
	// Popup ActionSelect Dlg
	//-----------------------------------------------------------------------------
	void				
	UIDialog::PopupActionSelectDlg(int x, int y)
	{
		//---------------------------------------------------------
		// 기존에 있던 dialog를 지운다.
		//---------------------------------------------------------
		if (m_pActionSelectDlg!=NULL)
		{
			delete m_pActionSelectDlg;
		}

	
		//---------------------------------------------------------
		// dialog 생성
		//---------------------------------------------------------
		m_pActionSelectDlg = new C_VS_UI_DIALOG(x, y, 1, 3, ProcessActionSelectDlg);//, SMO_NOFIT);

		//---------------------------------------------------------
		// 메뉴 구성..
		//---------------------------------------------------------
		const int menuSize = 11;//21;
		DIALOG_MENU menu[ menuSize ] = {
					{ "STAND",				0 }, 
					{ "MOVE", 				1 }, 
					{ "ATTACK", 			2 }, 
					{ "MAGIC", 				3 }, 
					{ "DAMAGED", 			4 }, 
					{ "DRAINED", 			5 }, 
					{ "DIE", 				6 }, 
					{ "VAMP_DRAIN",		7 }, 
					{ "THROW_WEAPON",	11 }, 
					{ "THROW_POTION",	17 },					
					
					/*
					{ "SLAYER_GUN_TR",		7 }, 
					{ "SLAYER_GUN_SG",		8 }, 
					{ "SLAYER_SWORD",		9 }, 
					{ "SLAYER_BLADE",		10 }, 
					{ "SLAYER_MOTOR_MOVE", 	12 }, 
					{ "SLAYER_GUN_AR", 		13 }, 
					{ "SLAYER_GUN_SMG", 	14 }, 
					{ "SLAYER_SWORD_2", 	15 }, 
					{ "SLAYER_BLADE_2", 	16 }, 
					{ "SLAYER_MOTOR_STAND", 18 },
					*/
					
					{"EXIT", DIALOG_EXECID_EXIT}
				};

		//---------------------------------------------------------
		// 메뉴 등록
		//---------------------------------------------------------
		m_pActionSelectDlg->SetMenu(menu, menuSize);		// 끝내기 포함

		//---------------------------------------------------------
		// dialog 시작..
		//---------------------------------------------------------
		m_pActionSelectDlg->Start();
	}

	//-----------------------------------------------------------------------------
	// Process ActionSelect Dlg
	//-----------------------------------------------------------------------------
	void			
	UIDialog::ProcessActionSelectDlg(C_VS_UI_DIALOG * pDlg, unsigned long id)
	{
		int actionID = id;

		switch (id)
		{
			case DIALOG_EXECID_EXIT :
			break;

			// Zone의 모든 캐릭터의 action을 바꾼다.
			default :
			{
				MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

				for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
				{
					MCreature* pCreature = (*iCreature).second;

					//--------------------------------------------------
					// player인 경우						
					//--------------------------------------------------
					if (pCreature->GetID()==g_pPlayer->GetID())
					{
						if (actionID==ACTION_DIE)
						{
							if (g_pPlayer->IsAlive())
							{
								g_pPlayer->SetDead();
							}
							else
							{
								g_pPlayer->SetAlive();
							}
						}
						else
						{
							// 그림이 있다면 action을 설정한다.
							g_pPlayer->SetAction( actionID );
						}
					}
					//--------------------------------------------------
					// NPC가 아닌 경우만...
					//--------------------------------------------------
					else if (!pCreature->IsNPC())
					{
						int body		= pCreature->GetCreatureFrameID(0);
						int action		= actionID;
						int	direction	= pCreature->GetDirection();
						
						if (body < g_pTopView->m_CreatureFPK.GetSize())
						{
							ACTION_FRAME_ARRAY& AFA = g_pTopView->m_CreatureFPK[body];
						
							if (action < AFA.GetSize())
							{
								DIRECTION_FRAME_ARRAY& DFA = AFA[action];
								if (direction < DFA.GetSize())
								{
									FRAME_ARRAY& FA = DFA[direction];
									if (FA.GetSize()!=0)
									{
										// 그림이 있다면 action을 설정한다.
										pCreature->SetNextAction( actionID );
									}
								}
							}
						}
					}

					iCreature++;
				}
			}
	
		}
	}


	//-----------------------------------------------------------------------------
	// Popup DirectionSelect Dlg
	//-----------------------------------------------------------------------------
	void				
	UIDialog::PopupDirectionSelectDlg(int x, int y)
	{
		//---------------------------------------------------------
		// 기존에 있던 dialog를 지운다.
		//---------------------------------------------------------
		if (m_pDirectionSelectDlg!=NULL)
		{
			delete m_pDirectionSelectDlg;
		}

	
		//---------------------------------------------------------
		// dialog 생성
		//---------------------------------------------------------
		m_pDirectionSelectDlg = new C_VS_UI_DIALOG(x, y, 1, 2, ProcessDirectionSelectDlg);//, SMO_NOFIT);

		//---------------------------------------------------------
		// 메뉴 구성..
		//---------------------------------------------------------
		const int menuSize = 9;//21;
		DIALOG_MENU menu[ menuSize ] = {
					{ "좌 (Left)",			0 }, 
					{ "좌하 (LeftDown)", 	1 }, 
					{ "하 (Down)", 			2 }, 
					{ "우하 (RightDown)",	3 }, 
					{ "우 (Right)", 		4 }, 
					{ "우상 (RightUp)", 	5 }, 
					{ "상 (Up)", 			6 }, 
					{ "좌상 (LeftUp)",		7 }, 
					
					
					{"EXIT", DIALOG_EXECID_EXIT}
				};

		//---------------------------------------------------------
		// 메뉴 등록
		//---------------------------------------------------------
		m_pDirectionSelectDlg->SetMenu(menu, menuSize);		// 끝내기 포함

		//---------------------------------------------------------
		// dialog 시작..
		//---------------------------------------------------------
		m_pDirectionSelectDlg->Start();
	}

	//-----------------------------------------------------------------------------
	// Process DirectionSelect Dlg
	//-----------------------------------------------------------------------------
	void			
	UIDialog::ProcessDirectionSelectDlg(C_VS_UI_DIALOG * pDlg, unsigned long id)
	{
		int direction = id;

		switch (id)
		{
			case DIALOG_EXECID_EXIT :
			break;

			// Zone의 모든 캐릭터의 Direction을 바꾼다.
			default :
			{
				MZone::CREATURE_MAP::const_iterator iCreature = g_pZone->GetCreatureBegin();

				for (int i=0; i<g_pZone->GetCreatureNumber(); i++)
				{
					MCreature* pCreature = (*iCreature).second;

					//--------------------------------------------------
					// NPC가 아닌 경우
					//--------------------------------------------------					
					if (!pCreature->IsNPC())
					{										
						pCreature->SetDirection( direction );									
					}

					iCreature++;
				}
			}
	
		}
	}
#endif
