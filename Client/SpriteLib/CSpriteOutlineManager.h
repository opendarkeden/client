#ifndef __CSPRITEOUTLINEMANAGER_H__
#define __CSPRITEOUTLINEMANAGER_H__

class CSprite;
class CIndexSprite;

#include <vector>

struct OutlineEntry {
	int x;
	int y;
	class CSprite* pSprite;
	class CIndexSprite* pIndexSprite;
	unsigned short color;
};

class CSpriteOutlineManager
{
public:
	CSpriteOutlineManager();
	~CSpriteOutlineManager();

	void Clear();
	bool IsInit() const { return !m_Outlines.empty(); } // Simple check if initialized
	void Add(int x, int y, class CSprite* pSprite);
	void Add(int x, int y, class CIndexSprite* pSprite, unsigned short color = 0); // color parameter now has default
	void Generate(int type = 0); // type parameter now has default

	// Get access to the outline entries for drawing sprites
	const std::vector<OutlineEntry>& GetOutlines() const { return m_Outlines; }

private:
	std::vector<OutlineEntry> m_Outlines;
};

#endif /* __CSPRITEOUTLINEMANAGER_H__ */
