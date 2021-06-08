#ifndef __MPALETTE_H__
#define __MPALETTE_H__

#include <windows.h>

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
	virtual bool	SaveToFile(class ofstream& file) { return false; };
	virtual bool	LoadFromFile(class ifstream& file) { return false; };		

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
	bool LoadFromFile(class ifstream &file);
	bool SaveToFile(class ofstream &file);
};

class MPalette565 : public MPalette
{
public:
	//--------------------------------------------------------
	// file I/O
	//--------------------------------------------------------
	bool LoadFromFile(class ifstream &file);
	bool SaveToFile(class ofstream &file);
};

#endif