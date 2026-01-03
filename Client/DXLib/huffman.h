#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <wtypes.h>
#include "bit_res.h"


#define HUFFBITS UINT
#define HTN	34
#define MXOFF	250

struct huffcodetab
{
  char tablename[3];			/* string, containing table_description   */
  UINT xlen; 				/* max. x-index+			      	         */
  UINT ylen;	            /* max. y-index+				               */
  UINT linbits; 			/* number of linbits			               */
  UINT linmax;				/* max number to be stored in linbits	   */
  int ref;					/* a positive value indicates a reference */
  HUFFBITS *table;			/* pointer to array[xlen][ylen]		      */
  unsigned char *hlen;		/* pointer to array[xlen][ylen]		      */
  unsigned char(*val)[2];	/* decoder tree				               */
  UINT treelen;				/* length of decoder tree		            */
};

extern struct huffcodetab ht[HTN];

int huffman_decoder(struct huffcodetab *h, int *x, int *y, int *v,
                      int *w, Bit_Reserve *br);

#endif
