// ActionKrToDk2.h: interface for the ActionKrToDk2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIONKRTODK2_H__18CC33BF_22AA_426E_AF6C_D77FFAB3BFAD__INCLUDED_)
#define AFX_ACTIONKRTODK2_H__18CC33BF_22AA_426E_AF6C_D77FFAB3BFAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef struct datatype
{
	BYTE tmp[14];
};


class ActionKrToDk2  
{
public:
	void SaveNameDefLog(CFile* file,int id);
	void SaveFormFile(CFile *file,bool mode);
	datatype* typetmp;
	int nsi;
	BYTE tmpKR_3;
	WORD tmpDK2_5;
	WORD tmpDK2_4;
	WORD wtmpDK2_3[5];
	char flag;
	BYTE tmpKR_2[1];
	int tmpKRlen;
	BYTE tmpKR_1[51];
	BYTE tmpDK2_2[100];
	BYTE tmpDK2_2_1[100];
	BYTE tmpDK2_2_2[100];
	BYTE tmpDK2_2_3[100];
	BYTE tmpDK2_1[100];
	char* m_name;
	int m_namelen;
	void LoadFormKrFile(CFile* file,bool mode);
	ActionKrToDk2();
	virtual ~ActionKrToDk2();

};

#endif // !defined(AFX_ACTIONKRTODK2_H__18CC33BF_22AA_426E_AF6C_D77FFAB3BFAD__INCLUDED_)
