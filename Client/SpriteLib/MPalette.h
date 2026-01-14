#ifndef __MPALETTE_H__
#define __MPALETTE_H__

#ifdef PLATFORM_WINDOWS
	#include <windows.h>
#else
	#include "../basic/Platform.h"
#endif

class MPalette
{
public:
	MPalette();
	~MPalette();
	
	//--------------------------------------------------------
	// Init/Release
	//--------------------------------------------------------
	void	Release();
	void	Init(BYTE size);
	
	//--------------------------------------------------------
	// Size
	//--------------------------------------------------------
	BYTE		GetSize() const { return m_Size; }
	
	//--------------------------------------------------------
	// operator
	//--------------------------------------------------------
	WORD&		operator [] (BYTE n)		{ return m_pColor[n]; }
	WORD&		operator [] (BYTE n) const { return m_pColor[n]; }
	void		operator = (const MPalette& pal);
	
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	virtual bool	SaveToFile(std::ofstream& file) { return false; };
	virtual bool	LoadFromFile(std::ifstream& file) { return false; };		

	bool IsInit() const { return (m_Size == 0)?false:true; }
	
protected:
	WORD *		m_pColor;
	BYTE		m_Size;
};

class MPalette555 : public MPalette
{
public:
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(std::ifstream &file);
	bool SaveToFile(std::ofstream &file);
};

class MPalette565 : public MPalette
{
public:
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(std::ifstream &file);
	bool SaveToFile(std::ofstream &file);
};

#endif