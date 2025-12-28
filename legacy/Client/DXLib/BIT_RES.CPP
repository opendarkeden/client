#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMCX
#define NOIME
#define NOGDI
#define NOUSER
#define NOSOUND
#define NOCOMM
#define NODRIVERS
#define OEMRESOURCE
#define NONLS
#define NOSERVICE
#define NOKANJI
#define NOMINMAX
#define NOLOGERROR
#define NOPROFILER
#define NOMEMMGR
#define NOLFILEIO
#define NOOPENFILE
#define NORESOURCE
#define NOATOM
#define NOLANGUAGE
#define NOLSTRING
#define NODBCS
#define NOKEYBOARDINFO
#define NOGDICAPMASKS
#define NOCOLOR
#define NOGDIOBJ
#define NODRAWTEXT
#define NOTEXTMETRIC
#define NOSCALABLEFONT
#define NOBITMAP
#define NORASTEROPS
#define NOMETAFILE
#define NOSYSMETRICS
#define NOSYSTEMPARAMSINFO
#define NOMSG
#define NOWINSTYLES
#define NOWINOFFSETS
#define NOSHOWWINDOW
#define NODEFERWINDOWPOS
#define NOVIRTUALKEYCODES
#define NOKEYSTATES
#define NOWH
#define NOMENUS
#define NOSCROLL
#define NOCLIPBOARD
#define NOICONS
#define NOMB
#define NOSYSCOMMANDS
#define NOMDI
#define NOCTLMGR
#define NOWINMESSAGES
#define NOHELP
#define _WINUSER_
#define __oleidl_h__
#define _OLE2_H_

#include "bit_res.h"

Bit_Reserve::Bit_Reserve ()
{
	offset = totbit = buf_byte_idx = 0;
	buf = new unsigned int[BUFSIZE];
	buf_bit_idx = 8;
	putmask = new int [10];
	putmask[0] = 0x000;
	putmask[1] = 0x001;
	putmask[2] = 0x003;
	putmask[3] = 0x007;
	putmask[4] = 0x00f;
	putmask[5] = 0x01f;
	putmask[6] = 0x03f;
	putmask[7] = 0x07f;
	putmask[8] = 0x0ff;
	putmask[9] = 0x1ff;
}

Bit_Reserve::~Bit_Reserve ()
{
	delete [] putmask;
	delete [] buf;
}

/*read N bit from the bit stream */
unsigned int Bit_Reserve::hgetbits(int N)
{
 unsigned int val=0;
 int j = N;
 int k, tmp;

 totbit += N;
 while (j > 0) {
	if (!buf_bit_idx) {
   	  buf_bit_idx = 8;
	     buf_byte_idx++;
/*	if (buf_byte_idx > offset)
	  { printf("Buffer overflow !!\n");exit(3); } */
	}

	k = ((unsigned int)j < buf_bit_idx) ? j : buf_bit_idx;
   /* BUFSIZE = 4096 = 2^12, so buf_byte_idx%BUFSIZE ==
      buf_byte_idx & 0xfff */
   tmp = buf[buf_byte_idx & 0xfff] & putmask[buf_bit_idx];
   buf_bit_idx -= k;
	tmp = tmp >> buf_bit_idx;
   j -=k;
   val |= tmp << j;
 }
 return(val);
}

/*write 8 bits into the bit stream */
void Bit_Reserve::hputbuf(unsigned int val)
{
  buf[offset & 0xfff] = val;
  offset++;
}

void Bit_Reserve::rewindNbits(int N)
{
	totbit -= N;
	buf_bit_idx += N;
	while( buf_bit_idx >= 8 )
	{  buf_bit_idx -= 8;
		buf_byte_idx--;
	}
}

void Bit_Reserve::rewindNbytes(int N)
{
   totbit -= (N << 3);
	buf_byte_idx -= N;
}
