//----------------------------------------------------------------------
// CSpritePackList.cpp
//----------------------------------------------------------------------

#include "CSprite.h"
#include "CSpritePackList.h"
#include <fstream>

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CSpritePackList::CSpritePackList()
{
}

CSpritePackList::~CSpritePackList()
{
	// list를 메모리에서 제거한다.
	Release();
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Release
//----------------------------------------------------------------------
void
CSpritePackList::Release()
{
	if (!m_listSprite.empty())
	{
		// 모든 CSprite를 지운다.
		SPRITE_LIST::iterator iSprite = m_listSprite.begin();

		// 각각의 CSprite의 메모리를 delete한다.
		while (iSprite != m_listSprite.end())
		{
			CSprite* pSprite = *iSprite;
			//pSprite->Release();
			delete pSprite;
			iSprite++;
		}

		m_listSprite.clear();
	}
}

//----------------------------------------------------------------------
// Add Sprite to List
//----------------------------------------------------------------------
// List의 끝에 CSprite*를 추가한다.
// pSprite는 외부에서 new를 해야하고 delete는 CSpritePackList 내부에서 한다.
//----------------------------------------------------------------------
void		
CSpritePackList::AddSprite(CSprite* pSprite)
{
	// list에 추가하고
	m_listSprite.insert(m_listSprite.end(), pSprite);
}

//----------------------------------------------------------------------
// Remove Sprite
//----------------------------------------------------------------------
// List의 Sprite중에서 n번째의 Sprite를 삭제한다.
// index는 0부터 nSprite-1까지이고 delete해줘야 한다.
//----------------------------------------------------------------------
void		
CSpritePackList::RemoveSprite(TYPE_SPRITEID n)
{
	// 삭제 불가
	if ( n >= m_listSprite.size() )
		return;

	SPRITE_LIST::iterator	iSprite = m_listSprite.begin();

	// n번째 sprite에 접근한다.
	for (TYPE_SPRITEID i=0; i<n; i++)
		iSprite++;

	// 메모리에서 지우고
	delete (*iSprite);

	// list에서 지운다.
	m_listSprite.erase(iSprite);
}

//----------------------------------------------------------------------
// Add Sprite
//----------------------------------------------------------------------
// n번째 뒤에 pSprite를 추가한다.
//
// ex)  0,1,2,3,4....,9  = 10개
//----------------------------------------------------------------------
void		
CSpritePackList::AddSprite(TYPE_SPRITEID n, CSprite* pSprite)
{	
	// n이 너무 클때
	if ( n > m_listSprite.size() )
		n = m_listSprite.size();

	SPRITE_LIST::iterator	iSprite = m_listSprite.begin();

	// n번째 sprite에 접근한다.
	for (TYPE_SPRITEID i=0; i<n; i++)
		iSprite++;

	// 추가한다.
	m_listSprite.insert(iSprite, pSprite);
}

//----------------------------------------------------------------------
// Get Sprite
//----------------------------------------------------------------------
// List의 Sprite중에서 n번째의 Sprite를 삭제한다.
// index는 0부터 nSprite-1까지이고 delete해줘야 한다.
//----------------------------------------------------------------------
CSprite*
CSpritePackList::GetSprite(TYPE_SPRITEID n) const
{
	// get 불가
	if ( n >= m_listSprite.size() )
		return NULL;

	SPRITE_LIST::const_iterator	iSprite = m_listSprite.begin();

	// n번째 sprite에 접근한다.
	for (TYPE_SPRITEID i=0; i<n; i++)
		iSprite++;

	// n번째 Sprite를 넘겨준다.
	return (*iSprite);
}

//----------------------------------------------------------------------
// Change Sprite
//----------------------------------------------------------------------
// n번째와 m번째 Sprite를 바꾼다.
//----------------------------------------------------------------------
void		
CSpritePackList::ChangeSprite(TYPE_SPRITEID n, TYPE_SPRITEID m)
{
	// 같은 경우엔 의미가 없겠지..
	if (m==n) return;

	// get 불가
	if ( n >= m_listSprite.size() || m >= m_listSprite.size())
		return;

	TYPE_SPRITEID temp;

	//------------------------------------------------------
	// n을 적은 수로 지정한다.
	//------------------------------------------------------
	if (n > m) 
	{
		temp = n;
		n = m;
		m = temp;
	}

	SPRITE_LIST::iterator	iFirstSprite = m_listSprite.begin();	

	//------------------------------------------------------
	// n번째 sprite에 접근한다.
	//------------------------------------------------------
	for (TYPE_SPRITEID i=0; i<n; i++)
		iFirstSprite++;

	//------------------------------------------------------
	// m번째 sprite에 접근한다.
	//------------------------------------------------------
	temp = m-n;
	SPRITE_LIST::iterator	iSecondSprite = iFirstSprite;
	for (int i=n; i<temp; i++)
		iSecondSprite++;
	
	//------------------------------------------------------
	// iFirstSprite와 iSecondSprite를 바꿔준다.
	//------------------------------------------------------
	CSprite*	pSprite = *iFirstSprite;
	*iFirstSprite = *iSecondSprite;
	*iSecondSprite = pSprite;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// list를 따라가면서 file에 저장해야한다.
//----------------------------------------------------------------------
bool		
CSpritePackList::SaveToFile(ofstream& spkFile, ofstream& indexFile)
{
	// 초기화되지 않았으면 
	if (m_listSprite.empty())
		return FALSE;	

	// Size 저장
	TYPE_SPRITEID size = m_listSprite.size();
	spkFile.write((const char *)&size, SIZE_SPRITEID); 
	indexFile.write((const char *)&size, SIZE_SPRITEID);

	//--------------------------------------------------
	// index file을 생성하기 위한 정보
	//--------------------------------------------------
	long*	pIndex = new long [size];

	// List의 모든 Sprite를 저장한다.
	SPRITE_LIST::iterator iSprite = m_listSprite.begin();

	//--------------------------------------------------
	// 모든 CSprite를 저장
	//--------------------------------------------------
	int i = 0;
	while (iSprite != m_listSprite.end())
	{
		// SpritePack file에 쓰여지는 index를 저장
		pIndex[i++] = spkFile.tellp();

		// Sprite 저장
		(*iSprite)->SaveToFile(spkFile);		// CSprite저장	
		iSprite ++;
	}
	
	//--------------------------------------------------
	// index 저장
	//--------------------------------------------------
	for (int i=0; i<size; i++)
	{
		indexFile.write((const char*)&pIndex[i], 4);
	}

	// 메모리에서 제거
	delete [] pIndex;

	return true;
}
