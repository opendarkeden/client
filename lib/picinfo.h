#ifndef __PICINFO_H__
#define __PICINFO_H__

/*-----------------------------------------------------------------------------
  Picture file info
-----------------------------------------------------------------------------*/
typedef struct
{
	BYTE *	p_data;    // pointer to a real image data uncompressed
	int		width;
	int		height;
   int		bpp;        // bit per pixel
   char *	filename;  // loaded filename included file extension name

} S_PICINFO;

#endif