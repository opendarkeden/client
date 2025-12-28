#include "mp3.h"
#include <wtypes.h>
#include "huffman.h"
#include <math.h>

struct
{
	int l[5];
	int s[3];
} sfbtable					= {{0, 6, 11, 16, 21}, {0, 6, 12}};

int		slen[2][16]			=	{{0, 0, 0, 0, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4},
								{0, 1, 2, 3, 0, 1, 2, 3, 1, 2, 3, 1, 2, 3, 2, 3}};

// 스케일 팩터를 설정한다.
void get_scale_factors(MP3 *mp3, int ch, int gr)
{
	int	sfb, window;
	gr_info_s*	gr_info		= &(mp3->si.ch[ch].gr[gr]);
	int scale_comp		= gr_info->scalefac_compress;
	int length0			= slen[0][scale_comp];
	int length1			= slen[1][scale_comp];

	if (gr_info->window_switching_flag && (gr_info->block_type == 2))
	{
		if (gr_info->mixed_block_flag)
		{/* MIXED */ /* NEW - ag 11/25 */
			for (sfb = 0; sfb < 8; sfb++)
			{
				mp3->scalefac[ch].l[sfb] = mp3->br->hgetbits(slen[0][gr_info->scalefac_compress]);
			}
			for (sfb = 3; sfb < 6; sfb++)
			{
				for (window=0; window<3; window++)
				{
					mp3->scalefac[ch].s[window][sfb] = mp3->br->hgetbits(slen[0][gr_info->scalefac_compress]);
				}
			}
			for (sfb = 6; sfb < 12; sfb++)
			{
				for (window=0; window<3; window++)
					mp3->scalefac[ch].s[window][sfb] = mp3->br->hgetbits(slen[1][gr_info->scalefac_compress]);
			}
			for (sfb=12,window=0; window<3; window++)
			{
				mp3->scalefac[ch].s[window][sfb] = 0;
			}
		}
		else
		{/* SHORT*/

			mp3->scalefac[ch].s[0][0]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[1][0]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[2][0]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[0][1]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[1][1]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[2][1]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[0][2]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[1][2]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[2][2]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[0][3]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[1][3]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[2][3]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[0][4]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[1][4]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[2][4]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[0][5]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[1][5]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[2][5]		= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].s[0][6]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[1][6]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[2][6]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[0][7]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[1][7]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[2][7]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[0][8]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[1][8]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[2][8]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[0][9]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[1][9]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[2][9]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[0][10]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[1][10]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[2][10]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[0][11]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[1][11]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[2][11]		= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].s[0][12]		= 0;
			mp3->scalefac[ch].s[1][12]		= 0;
			mp3->scalefac[ch].s[2][12]		= 0;
		}
	}
	else
	{	/* LONG types 0,1,3 */

		if ((mp3->si.ch[ch].scfsi[0] == 0) || (gr == 0))
		{
			mp3->scalefac[ch].l[0]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[1]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[2]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[3]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[4]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[5]			= mp3->br->hgetbits(length0);
		}

		if ((mp3->si.ch[ch].scfsi[1] == 0) || (gr == 0))
		{
			mp3->scalefac[ch].l[6]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[7]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[8]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[9]			= mp3->br->hgetbits(length0);
			mp3->scalefac[ch].l[10]			= mp3->br->hgetbits(length0);
		}

		if ((mp3->si.ch[ch].scfsi[2] == 0) || (gr == 0))
		{
			mp3->scalefac[ch].l[11]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[12]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[13]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[14]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[15]			= mp3->br->hgetbits(length1);
		}

		if ((mp3->si.ch[ch].scfsi[3] == 0) || (gr == 0))
		{
			mp3->scalefac[ch].l[16]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[17]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[18]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[19]			= mp3->br->hgetbits(length1);
			mp3->scalefac[ch].l[20]			= mp3->br->hgetbits(length1);
		}

		mp3->scalefac[ch].l[21]				= 0;
		mp3->scalefac[ch].l[22]				= 0;
	 }
}

int pretab[22]				= {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,3,3,3,2,0};

float two_pow2(int scale, int preflag,
								int pretab_offset, int l)
{
	int		index;
	static	float	two_to_negative_half_pow[40];
	static	BOOL	two_pow2_init			= FALSE;

	if (!two_pow2_init)
	{
		int i;

		for (i=0;i<40;i++)
			two_to_negative_half_pow[i]	= (float) pow(2.0, -0.5 * (double) i);

		two_pow2_init		= TRUE;
	}

	index					= l;

	if (preflag)
		index			   += pretab_offset;

	index					= index << scale;

	return(two_to_negative_half_pow[index]);
}


struct
{
	int	l[23];
	int s[14];
} sfBandIndex[3]			=
{{{0,4,8,12,16,20,24,30,36,44,52,62,74,90,110,134,162,196,238,288,342,418,576},
{0,4,8,12,16,22,30,40,52,66,84,106,136,192}},
{{0,4,8,12,16,20,24,30,36,42,50,60,72,88,106,128,156,190,230,276,330,384,576},
{0,4,8,12,16,22,28,38,50,64,80,100,126,192}},
{{0,4,8,12,16,20,24,30,36,44,54,66,82,102,126,156,194,240,296,364,448,550,576},
{0,4,8,12,16,22,30,42,58,78,104,138,180,192}}};



void dequntize_sample(MP3 *mp3, float xr[SBLIMIT][SSLIMIT] , int ch, int gr)
{
	// ////////////////////////////////////////
	side_info_t *si = &mp3->si ;
	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
	MP3Header *header = &mp3->Header ;
	///////////////////////////////

	int	ss, sb, cb=0, sfreq=header->Sample_Frequency ;
	int	next_cb_boundary, cb_begin, cb_width;
	float	temp;
	int	index=0, t_index;
	int	i;
	static BOOL	Dequant_init			= FALSE;
	static float	TO_FOUR_THIRDS[1024];

	if (!Dequant_init)
	{
		for(i = 0; i<1024; i++)
			TO_FOUR_THIRDS[i]			= (float) pow( (double) i, 4.0/3.0);

	Dequant_init = TRUE;
	}

	/* choose correct scalefactor band per block type, initalize boundary */

	if (gr_info->window_switching_flag && (gr_info->block_type == 2) )
		if (gr_info->mixed_block_flag)
			next_cb_boundary=sfBandIndex[sfreq].l[1];  /* LONG blocks: 0,1,3 */
		else
		{
			cb_width		= sfBandIndex[sfreq].s[1];
			next_cb_boundary= (cb_width << 2) - cb_width;
			cb_begin		= 0;
		}
	else
		next_cb_boundary	= sfBandIndex[sfreq].l[1];  /* LONG blocks: 0,1,3 */

   /* Compute overall (global) scaling. */

	temp					= (float) pow( 2.0 , (0.25 * (gr_info->global_gain - 210.0)));

	for (sb=0; sb < SBLIMIT; sb++)
		for(ss=0; ss < SSLIMIT; ss+=2)
		{
			xr[sb][ss]		= temp * TO_FOUR_THIRDS[abs(mp3->is[sb][ss])];
			xr[sb][ss+1]	= temp * TO_FOUR_THIRDS[abs(mp3->is[sb][ss+1])];
			if (mp3->is[sb][ss] < 0)
				xr[sb][ss]	= -xr[sb][ss];
			if (mp3->is[sb][ss+1] < 0)
				xr[sb][ss+1]= -xr[sb][ss+1];
		}

	/* apply formula per block type */

	for (sb=0 ; sb < SBLIMIT ; sb++)
	{
		for (ss=0 ; ss < SSLIMIT ; ss++)
		{
			if (index == next_cb_boundary)
			{ /* Adjust critical band boundary */
				if (gr_info->window_switching_flag && (gr_info->block_type == 2))
				{
					if (gr_info->mixed_block_flag)
					{
						if (index == sfBandIndex[sfreq].l[8])
						{
							next_cb_boundary		= sfBandIndex[sfreq].s[4];
							next_cb_boundary		= (next_cb_boundary << 2) - next_cb_boundary;
							cb						= 3;
							cb_width				= sfBandIndex[sfreq].s[4] - sfBandIndex[sfreq].s[3];
							cb_begin				= sfBandIndex[sfreq].s[3];
							cb_begin				= (cb_begin << 2) - cb_begin;
						}
						else if (index < sfBandIndex[sfreq].l[8])
							next_cb_boundary		= sfBandIndex[sfreq].l[(++cb)+1];
						else
						{
							next_cb_boundary		= sfBandIndex[sfreq].s[(++cb)+1];
							next_cb_boundary		= (next_cb_boundary << 2) - next_cb_boundary;
							cb_begin				= sfBandIndex[sfreq].s[cb];
							cb_width				= sfBandIndex[sfreq].s[cb+1] - cb_begin;
							cb_begin				= (cb_begin << 2) - cb_begin;
						}
					}
					else
					{
						next_cb_boundary			= sfBandIndex[sfreq].s[(++cb)+1];
						next_cb_boundary			= (next_cb_boundary << 2) - next_cb_boundary;
						cb_begin					= sfBandIndex[sfreq].s[cb];
						cb_width					= sfBandIndex[sfreq].s[cb+1] - cb_begin;
						cb_begin					= (cb_begin << 2) - cb_begin;
					}
				}
				else /* long blocks */
					next_cb_boundary				= sfBandIndex[sfreq].l[(++cb)+1];
			}
			/* Do long/short dependent scaling operations. */
			if (gr_info->window_switching_flag &&
				(((gr_info->block_type == 2) && (gr_info->mixed_block_flag == 0)) ||
				((gr_info->block_type == 2) && gr_info->mixed_block_flag && (sb >= 2))))
			{

				t_index		= (index - cb_begin) / cb_width;
				xr[sb][ss] *= (float)pow((double)2.0, 
				(((double)-2.0 * (double)gr_info->subblock_gain[t_index])
					- (double)((double)0.5 * ((double)1.0 + (double)gr_info->scalefac_scale)
					* (double)mp3->scalefac[ch].s[t_index][cb])));
			}
			else
			{   /* LONG block types 0,1,3 & 1st 2 subbands of switched blocks */
/*				xr[sb][ss] *= pow(2.0, -0.5 * (1.0+gr_info->scalefac_scale)
													 * (scalefac[ch].l[cb]
													 + gr_info->preflag * pretab[cb])); */
				xr[sb][ss] *= two_pow2(gr_info->scalefac_scale, gr_info->preflag, pretab[cb], mp3->scalefac[ch].l[cb]);
			}
			index++;
		}
	}
}

float TAN12[16]={(float)0.0,			(float)0.26794919,	(float)0.57735027,					(float)1.0,
                (float)1.73205081,	(float)3.73205081,	(float)9.9999999e10 /*unbounded*/,	(float)-3.73205081,
                (float)-1.73205081,	(float)-1.0,		(float)-0.57735027,					(float)-0.26794919,
                (float)0.0,			(float)0.26794919,	(float)0.57735027,					(float)1.0};

void stereodecode(MP3 *mp3, int gr)
{
	side_info_t	*si = &mp3->si ;
	MP3Header *header = &mp3->Header ;
	gr_info_s *gr_info = &(si->ch[0].gr[gr]);
	int sfreq = header->Sample_Frequency ;
    int mode_ext = header->ModeExt;
	
	int ms_stereo = (header->Mode == joint_stereo) && (header->ModeExt & 0x2);
	int i_stereo  = (header->Mode == joint_stereo) && (header->ModeExt & 0x1);
	int sfb;
	int i,j,sb,ss,is_pos[576];
	int lines, temp, temp2;
	float is_ratio[576];
	
	if  (mp3->channels == 1) { /* mono , bypass xr[0][][] to lr[0][][]*/
		
		for(sb=0;sb<SBLIMIT;sb++)
			for(ss=0;ss<SSLIMIT;ss+=3) {
				mp3->lr[0][sb][ss]   = mp3->ro[0][sb][ss];
				mp3->lr[0][sb][ss+1] = mp3->ro[0][sb][ss+1];
				mp3->lr[0][sb][ss+2] = mp3->ro[0][sb][ss+2];
			}
			
	} else {
		
		/* initialization */
		for (i=0; i<576; i+=8)
			is_pos[i] = is_pos[i+1] = is_pos[i+2] = is_pos[i+3] =
			is_pos[i+4] = is_pos[i+5] = is_pos[i+6] = is_pos[i+7] = 7;
		
		if (i_stereo)
		{  if (gr_info->window_switching_flag && (gr_info->block_type == 2))
		{  if( gr_info->mixed_block_flag )
		{  int max_sfb = 0;
		
		for (j=0; j<3; j++)
		{  int sfbcnt;
		sfbcnt = 2;
		for( sfb=12; sfb >=3; sfb-- )
		{
			i = sfBandIndex[sfreq].s[sfb];
			lines = sfBandIndex[sfreq].s[sfb+1] - i;
			i = (i << 2) - i + (j+1) * lines - 1;
			while ( lines > 0 )
			{  if ( mp3->ro[1][i/SSLIMIT][i%SSLIMIT] != 0.0f )
			{  sfbcnt = sfb;
			sfb = -10;
			lines = -10;
			}
			lines--;
			i--;
			}
		}
		sfb = sfbcnt + 1;
		
		if (sfb > max_sfb)
			max_sfb = sfb;
		
		while(sfb < 12)
		{	temp = sfBandIndex[sfreq].s[sfb];
		sb   = sfBandIndex[sfreq].s[sfb+1] - temp;
		i    = (temp << 2) - temp + j * sb;
		
		for ( ; sb > 0; sb--)
		{  is_pos[i] = mp3->scalefac[1].s[j][sfb];
		if (is_pos[i] != 7)
			is_ratio[i] = TAN12[is_pos[i]];
		
		i++;
		}
		sfb++;
		}
		sfb = sfBandIndex[sfreq].s[10];
		sb  = sfBandIndex[sfreq].s[11] - sfb;
		sfb = (sfb << 2) - sfb + j * sb;
		temp  = sfBandIndex[sfreq].s[11];
		sb = sfBandIndex[sfreq].s[12] - temp;
		i = (temp << 2) - temp + j * sb;
		
		for ( ; sb > 0; sb-- )
		{  is_pos[i] = is_pos[sfb];
		is_ratio[i] = is_ratio[sfb];
		i++;
		}
		}
		if (max_sfb <= 3)
		{  i = 2;
		ss = 17;
		sb = -1;
		while (i >= 0)
		{  if (mp3->ro[1][i][ss] != 0.0f)
		{
			sb = (i<<4) + (i<<1) + ss;
			i = -1;
		} else
		{  ss--;
							 if (ss < 0)
							 {  i--;
							 ss = 17;
							 }
		}
		}
		i = 0;
		while ( sfBandIndex[sfreq].l[i] <= sb )
			i++;
		sfb = i;
		i = sfBandIndex[sfreq].l[i];
		for (; sfb<8; sfb++)
		{  sb = sfBandIndex[sfreq].l[sfb+1]-sfBandIndex[sfreq].l[sfb];
		for (; sb>0; sb--)
		{  is_pos[i] = mp3->scalefac[1].l[sfb];
							 if (is_pos[i] != 7)
								 is_ratio[i] = TAN12[is_pos[i]];
							 
							 i++;
		}
		}
		}
		} else
		{  for (j=0; j<3; j++)
		{  int sfbcnt;
		sfbcnt = -1;
		for( sfb=12; sfb >=0; sfb-- )
		{
			temp = sfBandIndex[sfreq].s[sfb];
			lines = sfBandIndex[sfreq].s[sfb+1] - temp;
			i = (temp << 2) - temp + (j+1) * lines - 1;
			
			while ( lines > 0 )
			{  if (mp3->ro[1][i/SSLIMIT][i%SSLIMIT] != 0.0f)
			{  sfbcnt = sfb;
			sfb = -10;
			lines = -10;
			}
			lines--;
			i--;
			}
		}
		sfb = sfbcnt + 1;
		while( sfb<12 )
		{
			temp = sfBandIndex[sfreq].s[sfb];
			sb   = sfBandIndex[sfreq].s[sfb+1] - temp;
			i    = (temp << 2) - temp + j * sb;
			for ( ; sb > 0; sb--)
			{  is_pos[i] = mp3->scalefac[1].s[j][sfb];
			if (is_pos[i] != 7)
				is_ratio[i] = TAN12[is_pos[i]];
			
			i++;
			}
			sfb++;
		}
		
		temp = sfBandIndex[sfreq].s[10];
		temp2= sfBandIndex[sfreq].s[11];
		sb   = temp2 - temp;
		sfb  = (temp << 2) - temp + j * sb;
		sb   = sfBandIndex[sfreq].s[12] - temp2;
		i    = (temp2 << 2) - temp2 + j * sb;
		
		for (; sb>0; sb--)
		{  is_pos[i] = is_pos[sfb];
		is_ratio[i] = is_ratio[sfb];
		i++;
		}
		}
		}
		} else // ms-stereo
		{  i = 31;
		ss = 17;
		sb = 0;
		while (i >= 0)
		{  if (mp3->ro[1][i][ss] != 0.0f)
		{
			sb = (i<<4) + (i<<1) + ss;
			i = -1;
		} else
		{  ss--;
		if ( ss < 0 )
		{  i--;
		ss = 17;
		}
		}
		}
		i = 0;
		while ( sfBandIndex[sfreq].l[i] <= sb )
			i++;
		sfb = i;
		i = sfBandIndex[sfreq].l[i];
		for ( ; sfb<21; sfb++ )
		{  sb = sfBandIndex[sfreq].l[sfb+1] - sfBandIndex[sfreq].l[sfb];
		for ( ; sb > 0; sb--)
		{  is_pos[i] = mp3->scalefac[1].l[sfb];
		if ( is_pos[i] != 7 )
			is_ratio[i] = TAN12[is_pos[i]];
		
		i++;
		}
		}
		sfb = sfBandIndex[sfreq].l[20];
		for ( sb = 576 - sfBandIndex[sfreq].l[21]; sb > 0; sb-- )
		{  is_pos[i] = is_pos[sfb];
		is_ratio[i] = is_ratio[sfb];
		i++;
		}
		}
	}
	
   	i = 0;
	for(sb=0;sb<SBLIMIT;sb++)
		for(ss=0;ss<SSLIMIT;ss++) {
			if ( is_pos[i] == 7 ) {
				if (ms_stereo) {
					mp3->lr[0][sb][ss] = (mp3->ro[0][sb][ss]+mp3->ro[1][sb][ss])* 0.7071068f;
					mp3->lr[1][sb][ss] = (mp3->ro[0][sb][ss]-mp3->ro[1][sb][ss])* 0.7071068f;
				}
				else {
					mp3->lr[0][sb][ss] = mp3->ro[0][sb][ss];
					mp3->lr[1][sb][ss] = mp3->ro[1][sb][ss];
				}
			}
			else if (i_stereo ) {
				mp3->lr[1][sb][ss] = mp3->ro[0][sb][ss] / (float) (1 + is_ratio[i]);
				mp3->lr[0][sb][ss] = mp3->lr[1][sb][ss] * is_ratio[i];
			}
			/*				else {
			printf("Error in stereo processing\n");
		} */
            i++;
		}
		
    } // channels == 2
}

void reorder (MP3 *mp3, float xr[SBLIMIT][SSLIMIT], int ch, int gr)
{
	side_info_t *si = &mp3->si ;
	MP3Header *header = &mp3->Header ;

	gr_info_s*	gr_info		= &(si->ch[ch].gr[gr]);
	int		sfreq		= header->Sample_Frequency;
	int		sfb, sfb_start, sfb_lines;
	int		sb, ss, freq, src_line, des_line;

	if (gr_info->window_switching_flag && (gr_info->block_type == 2))
	{
		for(sb=0;sb<SBLIMIT;sb++)
			mp3->re_hybridOut[sb][0]			= mp3->re_hybridOut[sb][1]	= mp3->re_hybridOut[sb][2]	=
			mp3->re_hybridOut[sb][3]			= mp3->re_hybridOut[sb][4]	= mp3->re_hybridOut[sb][5]	=
			mp3->re_hybridOut[sb][6]			= mp3->re_hybridOut[sb][7]	= mp3->re_hybridOut[sb][8]	=
			mp3->re_hybridOut[sb][9]			= mp3->re_hybridOut[sb][10]	= mp3->re_hybridOut[sb][11]	=
			mp3->re_hybridOut[sb][12]		= mp3->re_hybridOut[sb][13]	= mp3->re_hybridOut[sb][14]	=
			mp3->re_hybridOut[sb][15]		= mp3->re_hybridOut[sb][16]	= mp3->re_hybridOut[sb][17]	= 0.0f;

		if (gr_info->mixed_block_flag)
		{
			/* NO REORDER FOR LOW 2 SUBBANDS */
			for (ss=0 ; ss < SSLIMIT ; ss+=3)
			{
				mp3->re_hybridOut[0][ss]		= xr[0][ss];
				mp3->re_hybridOut[0][ss+1]	= xr[0][ss+1];
				mp3->re_hybridOut[0][ss+2]	= xr[0][ss+2];
			}
			for (ss=0; ss < SSLIMIT; ss += 3)
			{
				mp3->re_hybridOut[1][ss]		= xr[1][ss];
				mp3->re_hybridOut[1][ss+1]	= xr[1][ss+1];
				mp3->re_hybridOut[1][ss+2]	= xr[1][ss+2];
			}

			/* REORDERING FOR REST SWITCHED SHORT */
			for(sfb = 3, sfb_start = sfBandIndex[sfreq].s[3],
				sfb_lines = sfBandIndex[sfreq].s[4] - sfb_start;
				sfb < 13; sfb++,sfb_start=sfBandIndex[sfreq].s[sfb],
				(sfb_lines=sfBandIndex[sfreq].s[sfb+1] - sfb_start))
			{
				int sfb_start3 = (sfb_start << 2) - sfb_start;

				for(freq=0;freq<sfb_lines;freq++)
				{
					int freq3			= (freq << 2) - freq;

					src_line			= sfb_start3 + freq;
					des_line			= sfb_start3 + freq3;
					mp3->re_hybridOut[des_line/SSLIMIT][des_line%SSLIMIT] = xr[src_line/SSLIMIT][src_line%SSLIMIT];
					src_line		   += sfb_lines;
					des_line++;
					mp3->re_hybridOut[des_line/SSLIMIT][des_line%SSLIMIT] = xr[src_line/SSLIMIT][src_line%SSLIMIT];
					src_line		   += sfb_lines;
					des_line++;
					mp3->re_hybridOut[des_line/SSLIMIT][des_line%SSLIMIT] = xr[src_line/SSLIMIT][src_line%SSLIMIT];
				}
			}
		}
		else
		{  /* pure short */
			for(sfb = 0, sfb_start = 0, sfb_lines = sfBandIndex[sfreq].s[1];
				sfb < 13;
				sfb++, sfb_start		= sfBandIndex[sfreq].s[sfb],
				(sfb_lines = sfBandIndex[sfreq].s[sfb+1] - sfb_start))
			{
				int sfb_start3		= (sfb_start << 2) - sfb_start;

				for(freq=0;freq<sfb_lines;freq++)
				{
					int freq3			= (freq << 2) - freq;
					src_line			= sfb_start3 + freq;
					des_line			= sfb_start3 + freq3;
					mp3->re_hybridOut[des_line/SSLIMIT][des_line%SSLIMIT] = xr[src_line/SSLIMIT][src_line%SSLIMIT];
					src_line += sfb_lines;
					des_line++;
					mp3->re_hybridOut[des_line/SSLIMIT][des_line%SSLIMIT] = xr[src_line/SSLIMIT][src_line%SSLIMIT];
					src_line += sfb_lines;
					des_line++;
					mp3->re_hybridOut[des_line/SSLIMIT][des_line%SSLIMIT] = xr[src_line/SSLIMIT][src_line%SSLIMIT];
				}
			}
		}
	}
	else
	{   /*long blocks */
		for (sb=0 ; sb < SBLIMIT ; sb++)
			for (ss = 0; ss < SSLIMIT; ss+=3)
			{
				mp3->re_hybridOut[sb][ss]	= xr[sb][ss];
				mp3->re_hybridOut[sb][ss+1]	= xr[sb][ss+1];
				mp3->re_hybridOut[sb][ss+2]	= xr[sb][ss+2];
		}
	}
}

float Ci[8]={-0.6f,-0.535f,-0.33f,-0.185f,-0.095f,-0.041f,-0.0142f,-0.0037f};

void antialias(MP3* mp3, int ch, int gr)
{
	side_info_t *si = &mp3->si ;
	gr_info_s *gr_info = &(si->ch[ch].gr[gr]);
	
	float			bu,bd;  /* upper and lower butterfly inputs */
	int				ss,sb;
	static float	ca[8],cs[8];
	static BOOL		antialias_init = FALSE;

	if (!antialias_init) {
		int i;
		float    sq;
		for (i=0;i<8;i++) {
			sq=(float)sqrt(1.0f+Ci[i]*Ci[i]);
			cs[i] = 1.0f/sq;
         ca[i] = Ci[i] * cs[i];
		}
		antialias_init = TRUE;
	}

	/* 31 alias-reduction operations between each pair of sub-bands */
	/* with 8 butterflies between each pair                         */

	if  (gr_info->window_switching_flag && (gr_info->block_type == 2) &&
		 !gr_info->mixed_block_flag ) {

   	for(sb=0;sb<SBLIMIT;sb++) {
        	mp3->hybridIn[sb][0] = mp3->re_hybridOut[sb][0];
        	mp3->hybridIn[sb][1] = mp3->re_hybridOut[sb][1];
        	mp3->hybridIn[sb][2] = mp3->re_hybridOut[sb][2];
        	mp3->hybridIn[sb][3] = mp3->re_hybridOut[sb][3];
        	mp3->hybridIn[sb][4] = mp3->re_hybridOut[sb][4];
        	mp3->hybridIn[sb][5] = mp3->re_hybridOut[sb][5];
        	mp3->hybridIn[sb][6] = mp3->re_hybridOut[sb][6];
        	mp3->hybridIn[sb][7] = mp3->re_hybridOut[sb][7];
        	mp3->hybridIn[sb][8] = mp3->re_hybridOut[sb][8];
        	mp3->hybridIn[sb][9] = mp3->re_hybridOut[sb][9];
        	mp3->hybridIn[sb][10]= mp3->re_hybridOut[sb][10];
        	mp3->hybridIn[sb][11]= mp3->re_hybridOut[sb][11];
        	mp3->hybridIn[sb][12]= mp3->re_hybridOut[sb][12];
        	mp3->hybridIn[sb][13]= mp3->re_hybridOut[sb][13];
        	mp3->hybridIn[sb][14]= mp3->re_hybridOut[sb][14];
        	mp3->hybridIn[sb][15]= mp3->re_hybridOut[sb][15];
       		mp3->hybridIn[sb][16]= mp3->re_hybridOut[sb][16];
        	mp3->hybridIn[sb][17]= mp3->re_hybridOut[sb][17];
      }

	} else if (gr_info->window_switching_flag && gr_info->mixed_block_flag &&
	  (gr_info->block_type == 2)) {

		mp3->hybridIn[0][0] = mp3->re_hybridOut[0][0];
		mp3->hybridIn[0][1] = mp3->re_hybridOut[0][1];
		mp3->hybridIn[0][2] = mp3->re_hybridOut[0][2];
		mp3->hybridIn[0][3] = mp3->re_hybridOut[0][3];
		mp3->hybridIn[0][4] = mp3->re_hybridOut[0][4];
		mp3->hybridIn[0][5] = mp3->re_hybridOut[0][5];
		mp3->hybridIn[0][6] = mp3->re_hybridOut[0][6];
		mp3->hybridIn[0][7] = mp3->re_hybridOut[0][7];
		mp3->hybridIn[0][8] = mp3->re_hybridOut[0][8];
		mp3->hybridIn[0][9] = mp3->re_hybridOut[0][9];

      for(ss=0;ss<8;ss++) {
			bu = mp3->re_hybridOut[0][17-ss];
			bd = mp3->re_hybridOut[1][ss];
			mp3->hybridIn[0][17-ss] = (bu * cs[ss]) - (bd * ca[ss]);
			mp3->hybridIn[1][ss]    = (bd * cs[ss]) + (bu * ca[ss]);
      }

      mp3->hybridIn[1][8]  = mp3->re_hybridOut[1][8];
      mp3->hybridIn[1][9]  = mp3->re_hybridOut[1][9];
      mp3->hybridIn[1][10] = mp3->re_hybridOut[1][10];
      mp3->hybridIn[1][11] = mp3->re_hybridOut[1][11];
      mp3->hybridIn[1][12] = mp3->re_hybridOut[1][12];
      mp3->hybridIn[1][13] = mp3->re_hybridOut[1][13];
      mp3->hybridIn[1][14] = mp3->re_hybridOut[1][14];
      mp3->hybridIn[1][15] = mp3->re_hybridOut[1][15];
      mp3->hybridIn[1][16] = mp3->re_hybridOut[1][16];
      mp3->hybridIn[1][17] = mp3->re_hybridOut[1][17];

		
	  for(sb=2;sb<SBLIMIT;sb++) {
        	mp3->hybridIn[sb][0] = mp3->re_hybridOut[sb][0];
        	mp3->hybridIn[sb][1] = mp3->re_hybridOut[sb][1];
        	mp3->hybridIn[sb][2] = mp3->re_hybridOut[sb][2];
        	mp3->hybridIn[sb][3] = mp3->re_hybridOut[sb][3];
        	mp3->hybridIn[sb][4] = mp3->re_hybridOut[sb][4];
        	mp3->hybridIn[sb][5] = mp3->re_hybridOut[sb][5];
        	mp3->hybridIn[sb][6] = mp3->re_hybridOut[sb][6];
        	mp3->hybridIn[sb][7] = mp3->re_hybridOut[sb][7];
        	mp3->hybridIn[sb][8] = mp3->re_hybridOut[sb][8];
        	mp3->hybridIn[sb][9] = mp3->re_hybridOut[sb][9];
        	mp3->hybridIn[sb][10]= mp3->re_hybridOut[sb][10];
        	mp3->hybridIn[sb][11]= mp3->re_hybridOut[sb][11];
        	mp3->hybridIn[sb][12]= mp3->re_hybridOut[sb][12];
        	mp3->hybridIn[sb][13]= mp3->re_hybridOut[sb][13];
        	mp3->hybridIn[sb][14]= mp3->re_hybridOut[sb][14];
        	mp3->hybridIn[sb][15]= mp3->re_hybridOut[sb][15];
        	mp3->hybridIn[sb][16]= mp3->re_hybridOut[sb][16];
        	mp3->hybridIn[sb][17]= mp3->re_hybridOut[sb][17];
      }

	} else {

      mp3->hybridIn[0][0] = mp3->re_hybridOut[0][0];
      mp3->hybridIn[0][1] = mp3->re_hybridOut[0][1];
      mp3->hybridIn[0][2] = mp3->re_hybridOut[0][2];
      mp3->hybridIn[0][3] = mp3->re_hybridOut[0][3];
      mp3->hybridIn[0][4] = mp3->re_hybridOut[0][4];
      mp3->hybridIn[0][5] = mp3->re_hybridOut[0][5];
      mp3->hybridIn[0][6] = mp3->re_hybridOut[0][6];
      mp3->hybridIn[0][7] = mp3->re_hybridOut[0][7];

		for(sb=0;sb<31;sb++) {
			for(ss=0;ss<8;ss++) {
				bu = mp3->re_hybridOut[sb][17-ss];
				bd = mp3->re_hybridOut[sb+1][ss];
				mp3->hybridIn[sb][17-ss]   = (bu * cs[ss]) - (bd * ca[ss]);
				mp3->hybridIn[sb+1][ss]    = (bd * cs[ss]) + (bu * ca[ss]);
			}

         mp3->hybridIn[sb][8]   = mp3->re_hybridOut[sb][8];
         mp3->hybridIn[sb][9]   = mp3->re_hybridOut[sb][9];
      }
      mp3->hybridIn[31][8]  = mp3->re_hybridOut[31][8];
      mp3->hybridIn[31][9]  = mp3->re_hybridOut[31][9];
      mp3->hybridIn[31][10] = mp3->re_hybridOut[31][10];
      mp3->hybridIn[31][11] = mp3->re_hybridOut[31][11];
      mp3->hybridIn[31][12] = mp3->re_hybridOut[31][12];
      mp3->hybridIn[31][13] = mp3->re_hybridOut[31][13];
      mp3->hybridIn[31][14] = mp3->re_hybridOut[31][14];
      mp3->hybridIn[31][15] = mp3->re_hybridOut[31][15];
      mp3->hybridIn[31][16] = mp3->re_hybridOut[31][16];
      mp3->hybridIn[31][17] = mp3->re_hybridOut[31][17];
   }
}


void hufman_decode(MP3 *mp3, int ch, int gr)
{
	side_info_t	*si = &mp3->si ;
	MP3Header *header = &mp3->Header ;
	int		i, x, y;
	int		v, w;
	int		part2_3_end	= mp3->part2Start + si->ch[ch].gr[gr].part2_3_length;
	int		num_bits;
	int		region1Start;
	int		region2Start;
	int		sf_index;
	int		ssindex, sbindex;
	struct	huffcodetab*	h;

	static BOOL huffman_init	= FALSE;

/*   if (!huffman_init)
	{
		if (read_decoder_table(huffdec_path))
			huffman_init = TRUE;
		else
			ExitThread(1L);
	} */

	/* Initialize output */
	/* Eliminates need to zero out the rest */

	for (x=0;x<SBLIMIT;x++)
	{
		mp3->is[x][0]			= 0;
		mp3->is[x][1]			= 0;
		mp3->is[x][2]			= 0;
		mp3->is[x][3]			= 0;
		mp3->is[x][4]			= 0;
		mp3->is[x][5]			= 0;
		mp3->is[x][6]			= 0;
		mp3->is[x][7]			= 0;
		mp3->is[x][8]			= 0;
		mp3->is[x][9]			= 0;
		mp3->is[x][10]			= 0;
		mp3->is[x][11]			= 0;
		mp3->is[x][12]			= 0;
		mp3->is[x][13]			= 0;
		mp3->is[x][14]			= 0;
		mp3->is[x][15]			= 0;
		mp3->is[x][16]			= 0;
		mp3->is[x][17]			= 0;
	}

	/* Find region boundary for short block case. */

	if ((si->ch[ch].gr[gr].window_switching_flag) &&
		(si->ch[ch].gr[gr].block_type == 2) )
	{
		/* Region2. */

		region1Start		= 36;  /* sfb[9/3]*3=36 */
		region2Start		= 576; /* No Region2 for short block case. */
	}
	else
	{          /* Find region boundary for long block case. */

		sf_index			= header->Sample_Frequency ;

		region1Start		= sfBandIndex[sf_index].l[si->ch[ch].gr[gr].region0_count + 1]; /* MI */
		region2Start		= sfBandIndex[sf_index].l[si->ch[ch].gr[gr].region0_count + si->ch[ch].gr[gr].region1_count + 2]; /* MI */
		}

	sbindex					= 0;
	ssindex					= 0;
	/* Read bigvalues area. */
	for (i=0; (unsigned int)i < (si->ch[ch].gr[gr].big_values << 1); i += 2)
	{
		if(i < region1Start)
			h				= &ht[si->ch[ch].gr[gr].table_select[0]];
		else if (i<region2Start)
			h				= &ht[si->ch[ch].gr[gr].table_select[1]];
		else
			h				= &ht[si->ch[ch].gr[gr].table_select[2]];

		huffman_decoder(h, &x, &y, &v, &w, mp3->br);

		mp3->is[sbindex][ssindex]= x;
		mp3->is[sbindex][ssindex+1]	= y;

		ssindex			   += 2;
		if (ssindex >= SSLIMIT)
		{
			ssindex			= 0;
			sbindex++;
		}
	}

	/* Read count1 area. */
	h						= &ht[(*si).ch[ch].gr[gr].count1table_select+32];
	num_bits				= mp3->br->hsstell();

	while ((num_bits < part2_3_end) && (sbindex < SBLIMIT))
	{
		huffman_decoder(h, &x, &y, &v, &w, mp3->br);
		mp3->is[sbindex][ssindex]			= v;
		mp3->is[sbindex][ssindex + 1]		= w;

		ssindex			   += 2;
		if (ssindex >= SSLIMIT)
		{
			ssindex			= 0;
			sbindex++;
		}

		if (sbindex < SBLIMIT)
		{
			mp3->is[sbindex][ssindex]		= x;
			mp3->is[sbindex][ssindex+1]		= y;
		}

		ssindex			   += 2;
		if (ssindex >= SSLIMIT)
		{
			ssindex			= 0;
			sbindex++;
		}
		num_bits			= mp3->br->hsstell();
	}

	if (num_bits > part2_3_end)
		mp3->br->rewindNbits(num_bits - part2_3_end);

	num_bits				= mp3->br->hsstell();

	/* Dismiss stuffing Bits */
	if (num_bits < part2_3_end)
		mp3->br->hgetbits(part2_3_end - num_bits);

	/* Zero out rest. */
/*	for (; i<SSLIMIT*SBLIMIT; i++)
		is[i/SSLIMIT][i%SSLIMIT] = 0; */
}




