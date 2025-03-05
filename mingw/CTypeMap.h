//----------------------------------------------------------------------
// CTypeMap.h
//----------------------------------------------------------------------
// DataType은 pointer type이어야 한다.
//----------------------------------------------------------------------

#ifndef __CTYPEMAP_H__
#define	__CTYPEMAP_H__

#pragma warning(disable:4786)

#include <map>
#include <fstream>
using std::begin, std::end;

//----------------------------------------------------------------------
// CTypeMap (id, DataType*)의 map
//----------------------------------------------------------------------
template <class DataType>
class CTypeMap : public std::map<unsigned int, DataType*> {
	public :
		typedef	std::map<unsigned int, DataType*>	TYPE_MAP;

	public :
		CTypeMap();
		virtual ~CTypeMap();

		//-----------------------------------------------------
		// Release
		//-----------------------------------------------------
		virtual void		Release();

		//-----------------------------------------------------
		// Item Info
		//-----------------------------------------------------
		virtual bool		AddData( unsigned int id, DataType* pData );
		virtual DataType*	GetData( unsigned int id );
		virtual bool		RemoveData( unsigned int id );

		//-----------------------------------------------------
		// File I/O
		//-----------------------------------------------------
		virtual void		SaveToFile(ofstream& file);
		virtual void		LoadFromFile(ifstream& file);

	protected :
};



//----------------------------------------------------------------------
//
// CTypeMap
//
//----------------------------------------------------------------------
template <class DataType>
CTypeMap<DataType>::CTypeMap()
{
}

template <class DataType>		
CTypeMap<DataType>::~CTypeMap()
{
	Release();
}

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
template <class DataType>
void
CTypeMap<DataType>::Release()
{
	typename TYPE_MAP::iterator iData = begin();

	while (iData != end())
	{
		DataType*	pData	= (*iData).second;		

		delete pData;

		iData ++;
	}

	clear();
}

//----------------------------------------------------------------------
// Add Data
//----------------------------------------------------------------------
template <class DataType>
bool				
CTypeMap<DataType>::AddData( unsigned int id, DataType* pData )
{
	TYPE_MAP::iterator	iData = find( id );

	if (iData != end())
	{
		// 이미 있는 경우
		// pData는 외부에서 지워줘야한다.
		return false;
	}

	//------------------------------------------------------
	// 없는 경우 --> 추가
	//------------------------------------------------------
	insert(TYPE_MAP::value_type(id, pData));

	return true;
}

//----------------------------------------------------------------------
// Get Data
//----------------------------------------------------------------------
template <class DataType>
DataType*	
CTypeMap<DataType>::GetData( unsigned int id )
{
	TYPE_MAP::iterator	iData = find( id );

	if (iData == end())
	{
		// 없는 경우 
		return NULL;
	}

	// 있는 경우
	return (*iData).second;
}

//----------------------------------------------------------------------
// Remove NPC Info
//----------------------------------------------------------------------
template <class DataType>
bool				
CTypeMap<DataType>::RemoveData( unsigned int id )
{
	TYPE_MAP::iterator	iData = find( id );

	if (iData == end())
	{
		// 없는 경우 
		return false;
	}

	// 있으면 지워줘야 한다.
	delete (*iData).second;

	erase( iData );

	return true;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
template <class DataType>
void		
CTypeMap<DataType>::SaveToFile(ofstream& file)
{
	TYPE_MAP::iterator iData = begin();

	//-----------------------------------------------------
	// 개수 저장
	//-----------------------------------------------------
	int infoSize = size();
	file.write((const char*)&infoSize, 4);

	//-----------------------------------------------------
	// 각 info 저장
	//-----------------------------------------------------
	while (iData != end())
	{
		unsigned int	id		= (*iData).first;
		DataType*		pData	= (*iData).second;		

		file.write((const char*)&id, 4);	// id 저장
		pData->SaveToFile( file );			// NPC info 저장

		iData ++;
	}
}
		
//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
template <class DataType>
void		
CTypeMap<DataType>::LoadFromFile(ifstream& file)
{
	//-----------------------------------------------------
	// 기존에 있던것 제거
	//-----------------------------------------------------
	Release();

	//-----------------------------------------------------
	// size 
	//-----------------------------------------------------
	int infoSize;
	file.read((char*)&infoSize, 4);

	//-----------------------------------------------------
	// 각 info
	//-----------------------------------------------------
	unsigned int id;
	for (int i=0; i<infoSize; i++)
	{		
		file.read((char*)&id, 4);
		DataType*	pData = new DataType;

		pData->LoadFromFile( file );

		//-----------------------------------------------------
		// map에 추가한다.
		//-----------------------------------------------------
		if (!AddData( id, pData ))
		{
			delete pData;
		}
	}
}



#endif

