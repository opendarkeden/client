//----------------------------------------------------------------------
// MNPCTable.h
//----------------------------------------------------------------------
/*
	int numberOfNPC;
	{
		int currentNPCID;
		
		int numberOfShopTemplate
		{
			int eachShopTemplateID
		} // 반복
		
		int DescriptionLength; // 설명의 길이
		string Description; // 설명 내용
		
	} // 반복

	참고로 상점이 없는 놈들은 numberOfShopTemplate가 0이다.
*/
//----------------------------------------------------------------------

#ifndef __MNPCTABLE_H__
#define	__MNPCTABLE_H__

#pragma warning(disable:4786)

#include "CtypeMap.h"
#include <list>
class ofstream;
class ifstream;
#include "MString.h"

//----------------------------------------------------------------------
// NPC_INFO
//----------------------------------------------------------------------
class NPC_INFO {
	public :
		typedef std::list<unsigned int>		SHOPTEMPLATEID_LIST;

	public :
		MString					Name;
		SHOPTEMPLATEID_LIST		ListShopTemplateID;

		MString					Description;
		int						SpriteID;

		//-----------------------------------------------------
		// Release
		//-----------------------------------------------------
		void		Release();

		//-----------------------------------------------------
		// File I/O
		//-----------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);
};

//----------------------------------------------------------------------
// MNPCTable (NPCID, NPC_INFO*)의 map
//----------------------------------------------------------------------
class MNPCTable : public CTypeMap<NPC_INFO> {
	public :
		MNPCTable();
		~MNPCTable();

	protected :
};

class	SERVERNPC_INFO {
	public :
		typedef std::list<unsigned int>		SHOPTEMPLATEID_LIST;

	public :
		SHOPTEMPLATEID_LIST		ListShopTemplateID;

		MString					Name;
		MString					Description;		

		//-----------------------------------------------------
		// Release
		//-----------------------------------------------------
		void		Release();

		//-----------------------------------------------------
		// File I/O
		//-----------------------------------------------------
		void		SaveToFile(class ofstream& file);
		void		LoadFromFile(class ifstream& file);
};


class MServerNPCTable : public CTypeMap<SERVERNPC_INFO> {
private :
	std::map<int,int>			NPCSPRITEID;
	
public :
	MServerNPCTable();
	~MServerNPCTable();

	int	AffectToNPCTable(MNPCTable* npc);
};


extern	MNPCTable*		g_pNPCTable;

#endif

