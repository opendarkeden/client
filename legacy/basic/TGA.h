/*-----------------------------------------------------------------------------

	TGA.h

	1999.5.	  KJTINC
	2000.8.29. 

-----------------------------------------------------------------------------*/

#ifndef __TARGA__
#define __TARGA__

#include "Basics.h"
#include "2D.h"
#include "DLL.h"

//----------------------------------------------------------------------------
// Tga head
//----------------------------------------------------------------------------
struct S_TGAHEAD
{
	BYTE idsize;
	BYTE colormaptype;  // is palette?
	BYTE imagetype;

	//
	// WORD colormapstart; 
	// WORD colormaplength
	//
	// `*to ignore VC word align!!!
	// `8bit image를 위한 것이므로 중요하지 않음!
	//
	//WORD colormapstart; 
	BYTE colormapstart1;
	BYTE colormapstart2;
	//WORD colormaplength; // *to ignore VC word align!!!
	BYTE colormaplength1;
	BYTE colormaplength2;

	BYTE colormapbits;
	WORD xstart;
	WORD ystart;
	WORD width;
	WORD height;
	BYTE bpp;   // bit per pixel
	BYTE descriptor;
};

//-----------------------------------------------------------------------------
// TGA class
//
// 하나의 TGA object.
//-----------------------------------------------------------------------------
class DllClass Tga
{
private:
	S_TGAHEAD					m_head;

	char *						m_sz_filename;
	BYTE *						m_p_data;

	int							m_colorkey;

	void	CopyLine(BYTE *dest, BYTE *src, int buf_length, int pixel_byte, bool reverse);
	void	TransformBit(int pixel_byte, WORD **p_data, int w, int h);
	void	ClearData();

public:

	Tga();
	~Tga();

	//
	// Getting the image info
	//
	// Old version: bool	Get_TGA_ImageInfo(char *sz_filename, S_PICINFO *picinfo);
	//
	int	Width() const { return m_head.width; }
	int	Height() const { return m_head.height; }
	int	BPP() const { return m_head.bpp; }
	const char * Filename() const { return m_sz_filename; }

	//
	// Loading TGA file
	//
	bool	Load(const char * sz_filename);
	bool	LoadButNoConvertTo565(const char * sz_filename);

	//
	// Blitting
	//
	void	Bltz(const S_SURFACEINFO * p_dest_surface, int x, int y);
	void	CkBltz(const S_SURFACEINFO * p_dest_surface, int x, int y);
	void	SetColorkey(int colorkey);

	//
	// Getting...
	//
	bool	GetSurfaceInfo(S_SURFACEINFO &surfaceinfo) const;
};

#endif