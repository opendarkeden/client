// ActionKrToDk2.cpp: implementation of the ActionKrToDk2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillTool.h"
#include "ActionKrToDk2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ActionKrToDk2::ActionKrToDk2()
{

}

ActionKrToDk2::~ActionKrToDk2()
{

}

void ActionKrToDk2::LoadFormKrFile(CFile *file,bool mode)
{
	BYTE ee[2]={0x0d,0x0a};
			file->Read(&m_namelen,4);
			m_name = new char[m_namelen+1];
			file->Read(m_name,m_namelen);
			m_name[m_namelen]=NULL;
		file->Read(&tmpDK2_1,68);
		//new kr  48+3+1
			if (mode)
			{
				file->Read(tmpKR_1,48);	
			}
		//
		file->Read(&tmpDK2_2,29);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
		//new kr  48+3+1
			//if (mode)
			{
			
				file->Read(&tmpKR_1,4);
			file->Read(&tmpDK2_2_1,11);
				tmpKRlen=0;
				file->Read(&tmpKRlen,1);
			file->Read(&tmpDK2_2_2,1);
				//file->Read(&tmpKR_2,1);
			}
		// end 
		
		flag = 0;
		file->Read((char*)&flag, sizeof(char) );
		bool m_bUseActionStep;
		m_bUseActionStep = (flag & 0x2) != 0;
		if( m_bUseActionStep )
		{
			for( int x = 0;x<5; x++)
			{
				file->Read(&wtmpDK2_3[x],2 );
			}
		}
		// new kr
			//if (mode)
			{
				for (int c=0;c<tmpKRlen;c++)
				{
					file->Read(&tmpKR_1,8);
				}
			}
			
		// end
		file->Read(&tmpDK2_4,2 );
		//file->Read(&tmp,2);
		file->Read(&tmpDK2_5,2);
		// new kr
			//if (mode)
			{	
				file->Read(&tmpKR_3,1);
			}
		//
		file->Read(&nsi,4);
		typetmp = new datatype[nsi];
		for (int h=0;h<nsi;h++)
		{
			file->Read(&typetmp[nsi].tmp,14);
		}
}

void ActionKrToDk2::SaveFormFile(CFile *file, bool mode)
{
	BYTE ee[2]={0x0d,0x0a};
			file->Write(&m_namelen,4);
			file->Write(m_name,m_namelen);
			m_name[m_namelen]=NULL;
		file->Write(&tmpDK2_1,68);
		//new kr  48+3+1
// 			if (mode)
// 			{
// 				file->Write(tmpKR_1,48);	
// 			}
		//
		file->Write(&tmpDK2_2,29);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
		//new kr  48+3+1
			//if (mode)
			{
			
/*				file->Write(&tmpKR_1,4);*/
			file->Write(&tmpDK2_2_1,11);
// 				tmpKRlen=0;
// 				file->Write(&tmpKRlen,1);
			file->Write(&tmpDK2_2_2,1);
				//file->Read(&tmpKR_2,1);
			}
		// end 
		
		flag = 0;
		file->Write((char*)&flag, sizeof(char) );
		bool m_bUseActionStep;
		m_bUseActionStep = (flag & 0x2) != 0;
		if( m_bUseActionStep )
		{
			for( int x = 0;x<5; x++)
			{
				file->Write(&wtmpDK2_3[x],2 );
			}
		}
		// new kr
			//if (mode)
// 			{
// 				for (int c=0;c<tmpKRlen;c++)
// 				{
// 					file->Write(&tmpKR_1,8);
// 				}
// 			}
			
		// end
		file->Write(&tmpDK2_4,2 );
		//file->Read(&tmp,2);
		file->Write(&tmpDK2_5,2);
		// new kr
			//if (mode)
// 			{	
// 				file->Write(&tmpKR_3,1);
// 			}
		//
		file->Write(&nsi,4);
		for (int h=0;h<nsi;h++)
		{
			file->Write(&typetmp[nsi].tmp,14);
		}
/*
			file->Write(&m_namelen,4);
			file->Write(m_name,m_namelen);
		file->Write(&tmpDK2_1,66);

		file->Write(&tmpDK2_2,43);
		//new kr  17 + 4 + 32=53
			if (mode)
			{
			
				file->Write(&tmpKR_1,51);
				file->Write(&tmpKRlen,1);
				file->Write(&tmpKR_2,1);
			}
		// end 
		
		file->Write((char*)&flag, sizeof(char) );
		bool m_bUseActionStep;
		m_bUseActionStep = (flag & 0x2) != 0;
		if( m_bUseActionStep )
		{
			for( int x = 0;x<5; x++)
			{
				file->Write(&wtmpDK2_3[x],2 );
			}
		}
		// new kr
			if (mode)
			{
				for (int c=0;c<tmpKRlen;c++)
				{
					file->Write(&tmpKR_1,8);
				}
			}
			
		// end
		file->Write(&tmpDK2_4,2 );
		//file->Read(&tmp,2);
		file->Write(&tmpDK2_5,2);
		// new kr
			if (mode)
			{	
				file->Write(&tmpKR_3,1);
			}
		//
		file->Write(&nsi,4);
		for (int h=0;h<nsi;h++)
		{
			file->Write(&typetmp[nsi].tmp,14);
		}
		*/
}

void ActionKrToDk2::SaveNameDefLog(CFile* file,int id)
{
	char buf[300];
	BYTE ee[2]={0x0d,0x0a};
	if (strcmp(m_name,"...Not Defined...")==0)
	{
		sprintf(buf,"SKILL_TEMP%d,//%d",id,id);
	}
	else
	{
		sprintf(buf,"%s,		//%d",m_name,id);
	}

	file->Write(buf,strlen(buf));
	file->Write(&ee[0],2);
}
