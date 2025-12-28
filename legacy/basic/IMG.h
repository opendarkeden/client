/*-----------------------------------------------------------------------------

	IMG.h

	Private Image data.

	2000.4.21. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __IMG_H__
#define __IMG_H__

#include "BasicS.h"
#include "2d.h"
#include "DLL.h"

/*-----------------------------------------------------------------------------
  Class IMG
-----------------------------------------------------------------------------*/
class DllClass C_IMG
{
private:
	BYTE *						m_p_data;
	char *						m_sz_filename;

	// IMG header
	struct S_IMGHEAD
	{
		WORD		bpp;
		WORD		width;	// image ≥–¿Ã
		WORD		height;	// image ≥Ù¿Ã

	} m_head;

public:
	C_IMG();
	~C_IMG();

	bool	Save(char * filename);
	//bool	LoadIMG(char * filename, S_PICINFO &p_picinfo);
	bool	Load(char * filename);
	//bool	LoadIMG(S_PICINFO &picinfo);

	// Getting...
	int	Width() const { return m_head.width; }
	int	Height() const { return m_head.height; }
	int	BPP() const { return m_head.bpp; }
	const char * Filename() const { return m_sz_filename; }
	bool	GetSurfaceInfo(S_SURFACEINFO &surfaceinfo) const;

	//void	Blt(S_SURFACEINFO &dest_surface, int x, int y);
};

#endif