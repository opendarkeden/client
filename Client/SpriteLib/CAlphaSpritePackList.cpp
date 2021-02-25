//----------------------------------------------------------------------
// CAlphaSpritePackList.cpp
//----------------------------------------------------------------------

#include "CAlphaSprite.h"
#include "CAlphaSpritePackList.h"
#include <fstream.h>

//----------------------------------------------------------------------
//
// constructor/destructor
//
//----------------------------------------------------------------------

CAlphaSpritePackList::CAlphaSpritePackList()
{
}

CAlphaSpritePackList::~CAlphaSpritePackList()
{
	// list�� �޸𸮿��� �����Ѵ�.
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
CAlphaSpritePackList::Release()
{
	if (!m_listSprite.empty())
	{
		// ��� CAlphaSprite�� �����.
		ALPHASPRITE_LIST::iterator iSprite = m_listSprite.begin();

		// ������ CAlphaSprite�� �޸𸮸� delete�Ѵ�.
		while (iSprite != m_listSprite.end())
		{
			delete (*iSprite);
			iSprite++;
		}

		m_listSprite.clear();
	}
}

//----------------------------------------------------------------------
// Add Sprite to List
//----------------------------------------------------------------------
// List�� ���� CAlphaSprite*�� �߰��Ѵ�.
// pSprite�� �ܺο��� new�� �ؾ��ϰ� delete�� CAlphaSpritePackList ���ο��� �Ѵ�.
//----------------------------------------------------------------------
void		
CAlphaSpritePackList::AddSprite(CAlphaSprite* pSprite)
{
	// list�� �߰��ϰ�
	m_listSprite.insert(m_listSprite.end(), pSprite);
}

//----------------------------------------------------------------------
// Remove Sprite
//----------------------------------------------------------------------
// List�� Sprite�߿��� n��°�� Sprite�� �����Ѵ�.
// index�� 0���� nSprite-1�����̰� delete����� �Ѵ�.
//----------------------------------------------------------------------
void		
CAlphaSpritePackList::RemoveSprite(TYPE_SPRITEID n)
{
	// ���� �Ұ�
	if ( n >= m_listSprite.size() )
		return;

	ALPHASPRITE_LIST::iterator	iSprite = m_listSprite.begin();

	// n��° sprite�� �����Ѵ�.
	for (TYPE_SPRITEID i=0; i<n; i++)
		iSprite++;

	// �޸𸮿��� �����
	delete (*iSprite);

	// list���� �����.
	m_listSprite.erase(iSprite);
}

//----------------------------------------------------------------------
// Add Sprite
//----------------------------------------------------------------------
// n��° �ڿ� pSprite�� �߰��Ѵ�.
//
// ex)  0,1,2,3,4....,9  = 10��
//----------------------------------------------------------------------
void		
CAlphaSpritePackList::AddSprite(TYPE_SPRITEID n, CAlphaSprite* pSprite)
{	
	// n�� �ʹ� Ŭ��
	if ( n > m_listSprite.size() )
		n = m_listSprite.size();

	ALPHASPRITE_LIST::iterator	iSprite = m_listSprite.begin();

	// n��° sprite�� �����Ѵ�.
	for (TYPE_SPRITEID i=0; i<n; i++)
		iSprite++;

	// �߰��Ѵ�.
	m_listSprite.insert(iSprite, pSprite);
}

//----------------------------------------------------------------------
// Get Sprite
//----------------------------------------------------------------------
// List�� Sprite�߿��� n��°�� Sprite�� �����Ѵ�.
// index�� 0���� nSprite-1�����̰� delete����� �Ѵ�.
//----------------------------------------------------------------------
CAlphaSprite*
CAlphaSpritePackList::GetSprite(TYPE_SPRITEID n) const
{
	// get �Ұ�
	if ( n >= m_listSprite.size() )
		return NULL;

	ALPHASPRITE_LIST::const_iterator	iSprite = m_listSprite.begin();

	// n��° sprite�� �����Ѵ�.
	for (TYPE_SPRITEID i=0; i<n; i++)
		iSprite++;

	// n��° Sprite�� �Ѱ��ش�.
	return (*iSprite);
}

//----------------------------------------------------------------------
// Change Sprite
//----------------------------------------------------------------------
// n��°�� m��° Sprite�� �ٲ۴�.
//----------------------------------------------------------------------
void		
CAlphaSpritePackList::ChangeSprite(TYPE_SPRITEID n, TYPE_SPRITEID m)
{
	// ���� ��쿣 �ǹ̰� ������..
	if (m==n) return;

	// get �Ұ�
	if ( n >= m_listSprite.size() || m >= m_listSprite.size())
		return;

	TYPE_SPRITEID temp;

	//------------------------------------------------------
	// n�� ���� ���� �����Ѵ�.
	//------------------------------------------------------
	if (n > m) 
	{
		temp = n;
		n = m;
		m = temp;
	}

	ALPHASPRITE_LIST::iterator	iFirstSprite = m_listSprite.begin();	

	//------------------------------------------------------
	// n��° sprite�� �����Ѵ�.
	//------------------------------------------------------
	for (TYPE_SPRITEID i=0; i<n; i++)
		iFirstSprite++;

	//------------------------------------------------------
	// m��° sprite�� �����Ѵ�.
	//------------------------------------------------------
	temp = m-n;
	ALPHASPRITE_LIST::iterator	iSecondSprite = iFirstSprite;
	for (i=n; i<temp; i++)
		iSecondSprite++;
	
	//------------------------------------------------------
	// iFirstSprite�� iSecondSprite�� �ٲ��ش�.
	//------------------------------------------------------
	CAlphaSprite*	pSprite = *iFirstSprite;
	*iFirstSprite = *iSecondSprite;
	*iSecondSprite = pSprite;
}

//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
// list�� ���󰡸鼭 file�� �����ؾ��Ѵ�.
//----------------------------------------------------------------------
bool		
CAlphaSpritePackList::SaveToFile(ofstream& spkFile, ofstream& indexFile)
{
	// �ʱ�ȭ���� �ʾ����� 
	if (m_listSprite.empty())
		return FALSE;

	// Size ����
	TYPE_SPRITEID size = m_listSprite.size();
	spkFile.write((const char *)&size, SIZE_SPRITEID); 
	indexFile.write((const char *)&size, SIZE_SPRITEID);

	//--------------------------------------------------
	// index file�� �����ϱ� ���� ����
	//--------------------------------------------------
	long*	pIndex = new long [size];


	// List�� ��� Sprite�� �����Ѵ�.
	ALPHASPRITE_LIST::iterator iSprite = m_listSprite.begin();

	//--------------------------------------------------
	// ��� CAlphaSprite�� ����
	//--------------------------------------------------
	int i = 0;
	while (iSprite != m_listSprite.end())
	{
		// SpritePack file�� �������� index�� ����
		pIndex[i++] = spkFile.tellp();

		// Sprite ����
		(*iSprite)->SaveToFile(spkFile);		// CAlphaSprite����	
		iSprite ++;
	}
	
	//--------------------------------------------------
	// index ����
	//--------------------------------------------------
	for (i=0; i<size; i++)
	{
		indexFile.write((const char*)&pIndex[i], 4);
	}

	delete [] pIndex;

	return true;
}