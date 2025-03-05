#ifndef L3TYPE_H
#define L3TYPE_H


typedef	struct {
				UINT part2_3_length;
				UINT big_values;
				UINT global_gain;
				UINT scalefac_compress;
				UINT window_switching_flag;
				UINT block_type;
				UINT mixed_block_flag;
				UINT table_select[3];
				UINT subblock_gain[3];
				UINT region0_count;
				UINT region1_count;
				UINT preflag;
				UINT scalefac_scale;
				UINT count1table_select;
} gr_info_s;


typedef struct {
	UINT main_data_begin;
	UINT private_bits;
	struct {
		UINT scfsi[4];
		gr_info_s gr[2];
	} ch[2];
} side_info_t;


typedef struct
{
	int l[23];            /* [cb] */
	int s[3][13];         /* [window][cb] */
} scalefac_t[2];

#endif