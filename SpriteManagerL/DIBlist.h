#ifndef _DIBLIST_H
#define _DIBLIST_H

#include"stdafx.h"

#include <afxtempl.h>

#include "CDib.h"




class CDIBList : public CList< CDib*, CDib*>
{
public:
	~CDIBList()
	{
		RemoveAll();
	};

	// Copy all element
	void operator=( const CDIBList& s)
	{
		CDIBList::RemoveAll();
		POSITION pos;
		pos = s.GetHeadPosition();
		while( pos)
		{
			CDib* pDib = s.GetNext( pos);
			CDib* pNewDib = new CDib( *pDib);
			AddTail( pNewDib);
		}

	}


	CDib* GetAt2( int i)
	{
		POSITION pos = CList< CDib*, CDib*>::FindIndex( i);
		if( pos == NULL)
			return NULL;
		else
			return CList< CDib*, CDib*>::GetAt( pos);

	};

	void RemoveAll()
	{
		// release 
		POSITION pos;
		pos = CList< CDib*, CDib*>::GetHeadPosition();
		while( pos)
		{
			CDib* pDib = CList< CDib*, CDib*>::GetNext( pos);
			delete pDib;
		}

		CList< CDib*, CDib*>::RemoveAll();
	};

	void RemoveAt( int i)
	{
		POSITION pos = CList< CDib*, CDib*>::FindIndex( i);

		CDib* pDib = CList< CDib*, CDib*>::GetAt( pos);
		delete pDib;

		CList< CDib*, CDib*>::RemoveAt( pos);

	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	sprite manager의 data 형식에 맞는 string으로부터 dib들을 읽어 들인다...
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void LoadDIBs( const char* data, int repeat = 1)	// repeat개 만큼 중복..
	{
		RemoveAll();

//		int t = 0;

		// 읽기.
		char szFileName[256];
		int bmps  = (int)*data++;

		if( repeat != 1 && bmps != 1)
		{
			MessageBox( NULL, "Error!!!!", "NO", MB_OK);
			return;
		}
//		data++;
		for( int i = 0; i< bmps; i++)
		{
			data++;	// skip '<'
			int fileNameLen = strchr( data, '>') - data;
			if( fileNameLen <= 0) return;	// error...

			memset( szFileName, 0, sizeof( szFileName));
			strncpy( szFileName, data, fileNameLen);

			// load ani list..
			CDib* pDib = new CDib;
			if( pDib->LoadBitmapFile( szFileName, FALSE) == FALSE)
			{
				char msg[256];
				sprintf( msg, "%s(%s)", "CDIBList::LoadDIBs Error. 화일을 열 수 없습니다.", szFileName);
				AfxMessageBox( msg);
				delete pDib;
				pDib = NULL;
			}

			// fix me
			for( int r = 0; r <repeat ; r++)
				CList< CDib*, CDib*>::AddTail( pDib);

			data+= fileNameLen+1;
		}
	}
};



#endif