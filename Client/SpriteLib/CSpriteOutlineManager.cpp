#include "CSpriteOutlineManager.h"
#include "CSprite.h"
#include "CIndexSprite.h"

CSpriteOutlineManager::CSpriteOutlineManager()
{
	m_Outlines.clear();
}

CSpriteOutlineManager::~CSpriteOutlineManager()
{
	Clear();
}

void CSpriteOutlineManager::Clear()
{
	m_Outlines.clear();
}

void CSpriteOutlineManager::Add(int x, int y, CSprite* pSprite)
{
	OutlineEntry entry;
	entry.x = x;
	entry.y = y;
	entry.pSprite = pSprite;
	entry.pIndexSprite = NULL;
	entry.color = 0;
	m_Outlines.push_back(entry);
}

void CSpriteOutlineManager::Add(int x, int y, CIndexSprite* pSprite, unsigned short color)
{
	OutlineEntry entry;
	entry.x = x;
	entry.y = y;
	entry.pSprite = NULL;
	entry.pIndexSprite = pSprite;
	entry.color = color;
	m_Outlines.push_back(entry);
}

void CSpriteOutlineManager::Generate(int type)
{
	// TODO: Generate sprite outline
	// For now, this is a stub
	(void)type;
}
