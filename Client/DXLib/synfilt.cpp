#include <math.h>
#include "mp3.h"
#include "soundbuf.h"

static const float MY_PI = (const float)3.14159265358979323846;

static const float cos1_64  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI					   / (const float)64.0));
static const float cos3_64  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)3.0  / (const float)64.0));
static const float cos5_64  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)5.0  / (const float)64.0));
static const float cos7_64  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)7.0  / (const float)64.0));
static const float cos9_64  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)9.0  / (const float)64.0));
static const float cos11_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)11.0 / (const float)64.0));
static const float cos13_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)13.0 / (const float)64.0));
static const float cos15_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)15.0 / (const float)64.0));
static const float cos17_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)17.0 / (const float)64.0));
static const float cos19_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)19.0 / (const float)64.0));
static const float cos21_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)21.0 / (const float)64.0));
static const float cos23_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)23.0 / (const float)64.0));
static const float cos25_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)25.0 / (const float)64.0));
static const float cos27_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)27.0 / (const float)64.0));
static const float cos29_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)29.0 / (const float)64.0));
static const float cos31_64 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)31.0 / (const float)64.0));
static const float cos1_32  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI					   / (const float)32.0));
static const float cos3_32  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)3.0  / (const float)32.0));
static const float cos5_32  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)5.0  / (const float)32.0));
static const float cos7_32  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)7.0  / (const float)32.0));
static const float cos9_32  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)9.0  / (const float)32.0));
static const float cos11_32 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)11.0 / (const float)32.0));
static const float cos13_32 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)13.0 / (const float)32.0));
static const float cos15_32 = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)15.0 / (const float)32.0));
static const float cos1_16  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI					   / (const float)16.0));
static const float cos3_16  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)3.0  / (const float)16.0));
static const float cos5_16  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)5.0  / (const float)16.0));
static const float cos7_16  = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)7.0  / (const float)16.0));
static const float cos1_8   = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI					   / (const float)8.0));
static const float cos3_8   = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI * (const float)3.0  / (const float)8.0));
static const float cos1_4   = (const float)1.0 / ((const float)2.0 * (const float)cos(MY_PI					   / (const float)4.0));


const float d[512] = {
  // Note: These values are not in the same order
  // as in Annex 3-B.3 of the ISO/IEC DIS 11172-3
	(const float)0.000000000,	(const float)-0.000442505,	(const float)0.003250122,	(const float)-0.007003784,
	(const float)0.031082153,	(const float)-0.078628540,	(const float)0.100311279,	(const float)-0.572036743,
	(const float)1.144989014,	(const float)0.572036743,	(const float)0.100311279,	(const float)0.078628540,
	(const float)0.031082153,	(const float)0.007003784,	(const float)0.003250122,	(const float)0.000442505,
	(const float)-0.000015259,	(const float)-0.000473022,	(const float)0.003326416,	(const float)-0.007919312,
	(const float)0.030517578,	(const float)-0.084182739,	(const float)0.090927124,	(const float)-0.600219727,
	(const float)1.144287109,	(const float)0.543823242,	(const float)0.108856201,	(const float)0.073059082,
	(const float)0.031478882,	(const float)0.006118774,	(const float)0.003173828,	(const float)0.000396729,
	(const float)-0.000015259,	(const float)-0.000534058,	(const float)0.003387451,	(const float)-0.008865356,
	(const float)0.029785156,	(const float)-0.089706421,	(const float)0.080688477,	(const float)-0.628295898,
	(const float)1.142211914,	(const float)0.515609741,	(const float)0.116577148,	(const float)0.067520142,
	(const float)0.031738281,	(const float)0.005294800,	(const float)0.003082275,	(const float)0.000366211,
	(const float)-0.000015259,	(const float)-0.000579834,	(const float)0.003433228,	(const float)-0.009841919,
	(const float)0.028884888,	(const float)-0.095169067,	(const float)0.069595337,	(const float)-0.656219482,
	(const float)1.138763428,	(const float)0.487472534,	(const float)0.123474121,	(const float)0.061996460,
	(const float)0.031845093,	(const float)0.004486084,	(const float)0.002990723,	(const float)0.000320435,
	(const float)-0.000015259,	(const float)-0.000625610,	(const float)0.003463745,	(const float)-0.010848999,
	(const float)0.027801514,	(const float)-0.100540161,	(const float)0.057617188,	(const float)-0.683914185,
	(const float)1.133926392,	(const float)0.459472656,	(const float)0.129577637,	(const float)0.056533813,
	(const float)0.031814575,	(const float)0.003723145,	(const float)0.002899170,	(const float)0.000289917,
	(const float)-0.000015259,	(const float)-0.000686646,	(const float)0.003479004,	(const float)-0.011886597,
	(const float)0.026535034,	(const float)-0.105819702,	(const float)0.044784546,	(const float)-0.711318970,
	(const float)1.127746582,	(const float)0.431655884,	(const float)0.134887695,	(const float)0.051132202,
	(const float)0.031661987,	(const float)0.003005981,	(const float)0.002792358,	(const float)0.000259399,
	(const float)-0.000015259,	(const float)-0.000747681,	(const float)0.003479004,	(const float)-0.012939453,
	(const float)0.025085449,	(const float)-0.110946655,	(const float)0.031082153,	(const float)-0.738372803,
	(const float)1.120223999,	(const float)0.404083252,	(const float)0.139450073,	(const float)0.045837402,
	(const float)0.031387329,	(const float)0.002334595,	(const float)0.002685547,	(const float)0.000244141,
	(const float)-0.000030518,	(const float)-0.000808716,	(const float)0.003463745,	(const float)-0.014022827,
	(const float)0.023422241,	(const float)-0.115921021,	(const float)0.016510010,	(const float)-0.765029907,
	(const float)1.111373901,	(const float)0.376800537,	(const float)0.143264771,	(const float)0.040634155,
	(const float)0.031005859,	(const float)0.001693726,	(const float)0.002578735,	(const float)0.000213623,
	(const float)-0.000030518,	(const float)-0.000885010,	(const float)0.003417969,	(const float)-0.015121460,
	(const float)0.021575928,	(const float)-0.120697021,	(const float)0.001068115,	(const float)-0.791213989,
	(const float)1.101211548,	(const float)0.349868774,	(const float)0.146362305,	(const float)0.035552979,
	(const float)0.030532837,	(const float)0.001098633,	(const float)0.002456665,	(const float)0.000198364,
	(const float)-0.000030518,	(const float)-0.000961304,	(const float)0.003372192,	(const float)-0.016235352,
	(const float)0.019531250,	(const float)-0.125259399,	(const float)-0.015228271,	(const float)-0.816864014,
	(const float)1.089782715,	(const float)0.323318481,	(const float)0.148773193,	(const float)0.030609131,
	(const float)0.029937744,	(const float)0.000549316,	(const float)0.002349854,	(const float)0.000167847,
	(const float)-0.000030518,	(const float)-0.001037598,	(const float)0.003280640,	(const float)-0.017349243,
	(const float)0.017257690,	(const float)-0.129562378,	(const float)-0.032379150,	(const float)-0.841949463,
	(const float)1.077117920,	(const float)0.297210693,	(const float)0.150497437,	(const float)0.025817871,
	(const float)0.029281616,	(const float)0.000030518,	(const float)0.002243042,	(const float)0.000152588,
	(const float)-0.000045776,	(const float)-0.001113892,	(const float)0.003173828,	(const float)-0.018463135,
	(const float)0.014801025,	(const float)-0.133590698,	(const float)-0.050354004,	(const float)-0.866363525,
	(const float)1.063217163,	(const float)0.271591187,	(const float)0.151596069,	(const float)0.021179199,
	(const float)0.028533936,	(const float)-0.000442505,	(const float)0.002120972,	(const float)0.000137329,
	(const float)-0.000045776,	(const float)-0.001205444,	(const float)0.003051758,	(const float)-0.019577026,
	(const float)0.012115479,	(const float)-0.137298584,	(const float)-0.069168091,	(const float)-0.890090942,
	(const float)1.048156738,	(const float)0.246505737,	(const float)0.152069092,	(const float)0.016708374,
	(const float)0.027725220,	(const float)-0.000869751,	(const float)0.002014160,	(const float)0.000122070,
	(const float)-0.000061035,	(const float)-0.001296997,	(const float)0.002883911,	(const float)-0.020690918,
	(const float)0.009231567,	(const float)-0.140670776,	(const float)-0.088775635,	(const float)-0.913055420,
	(const float)1.031936646,	(const float)0.221984863,	(const float)0.151962280,	(const float)0.012420654,
	(const float)0.026840210,	(const float)-0.001266479,	(const float)0.001907349,	(const float)0.000106812,
	(const float)-0.000061035,	(const float)-0.001388550,	(const float)0.002700806,	(const float)-0.021789551,
	(const float)0.006134033,	(const float)-0.143676758,	(const float)-0.109161377,	(const float)-0.935195923,
	(const float)1.014617920,	(const float)0.198059082,	(const float)0.151306152,	(const float)0.008316040,
	(const float)0.025909424,	(const float)-0.001617432,	(const float)0.001785278,	(const float)0.000106812,
	(const float)-0.000076294,	(const float)-0.001480103,	(const float)0.002487183,	(const float)-0.022857666,
	(const float)0.002822876,	(const float)-0.146255493,	(const float)-0.130310059,	(const float)-0.956481934,
	(const float)0.996246338,	(const float)0.174789429,	(const float)0.150115967,	(const float)0.004394531,
	(const float)0.024932861,	(const float)-0.001937866,	(const float)0.001693726,	(const float)0.000091553,
	(const float)-0.000076294,	(const float)-0.001586914,	(const float)0.002227783,	(const float)-0.023910522,
	(const float)-0.000686646,	(const float)-0.148422241,	(const float)-0.152206421,	(const float)-0.976852417,
	(const float)0.976852417,	(const float)0.152206421,	(const float)0.148422241,	(const float)0.000686646,
	(const float)0.023910522,	(const float)-0.002227783,	(const float)0.001586914,	(const float)0.000076294,
	(const float)-0.000091553,	(const float)-0.001693726,	(const float)0.001937866,	(const float)-0.024932861,
	(const float)-0.004394531,	(const float)-0.150115967,	(const float)-0.174789429,	(const float)-0.996246338,
	(const float)0.956481934,	(const float)0.130310059,	(const float)0.146255493,	(const float)-0.002822876,
	(const float)0.022857666,	(const float)-0.002487183,	(const float)0.001480103,	(const float)0.000076294,
	(const float)-0.000106812,	(const float)-0.001785278,	(const float)0.001617432,	(const float)-0.025909424,
	(const float)-0.008316040,	(const float)-0.151306152,	(const float)-0.198059082,	(const float)-1.014617920,
	(const float)0.935195923,	(const float)0.109161377,	(const float)0.143676758,	(const float)-0.006134033,
	(const float)0.021789551,	(const float)-0.002700806,	(const float)0.001388550,	(const float)0.000061035,
	(const float)-0.000106812,	(const float)-0.001907349,	(const float)0.001266479,	(const float)-0.026840210,
	(const float)-0.012420654,	(const float)-0.151962280,	(const float)-0.221984863,	(const float)-1.031936646,
	(const float)0.913055420,	(const float)0.088775635,	(const float)0.140670776,	(const float)-0.009231567,
	(const float)0.020690918,	(const float)-0.002883911,	(const float)0.001296997,	(const float)0.000061035,
	(const float)-0.000122070,	(const float)-0.002014160,	(const float)0.000869751,	(const float)-0.027725220,
	(const float)-0.016708374,	(const float)-0.152069092,	(const float)-0.246505737,	(const float)-1.048156738,
	(const float)0.890090942,	(const float)0.069168091,	(const float)0.137298584,	(const float)-0.012115479,
	(const float)0.019577026,	(const float)-0.003051758,	(const float)0.001205444,	(const float)0.000045776,
	(const float)-0.000137329,	(const float)-0.002120972,	(const float)0.000442505,	(const float)-0.028533936,
	(const float)-0.021179199,	(const float)-0.151596069,	(const float)-0.271591187,	(const float)-1.063217163,
	(const float)0.866363525,	(const float)0.050354004,	(const float)0.133590698,	(const float)-0.014801025,
	(const float)0.018463135,	(const float)-0.003173828,	(const float)0.001113892,	(const float)0.000045776,
	(const float)-0.000152588,	(const float)-0.002243042,	(const float)-0.000030518,	(const float)-0.029281616,
	(const float)-0.025817871,	(const float)-0.150497437,	(const float)-0.297210693,	(const float)-1.077117920,
	(const float)0.841949463,	(const float)0.032379150,	(const float)0.129562378,	(const float)-0.017257690,
	(const float)0.017349243,	(const float)-0.003280640,	(const float)0.001037598,	(const float)0.000030518,
	(const float)-0.000167847,	(const float)-0.002349854,	(const float)-0.000549316,	(const float)-0.029937744,
	(const float)-0.030609131,	(const float)-0.148773193,	(const float)-0.323318481,	(const float)-1.089782715,
	(const float)0.816864014,	(const float)0.015228271,	(const float)0.125259399,	(const float)-0.019531250,
	(const float)0.016235352,	(const float)-0.003372192,	(const float)0.000961304,	(const float)0.000030518,
	(const float)-0.000198364,	(const float)-0.002456665,	(const float)-0.001098633,	(const float)-0.030532837,
	(const float)-0.035552979,	(const float)-0.146362305,	(const float)-0.349868774,	(const float)-1.101211548,
	(const float)0.791213989,	(const float)-0.001068115,	(const float)0.120697021,	(const float)-0.021575928,
	(const float)0.015121460,	(const float)-0.003417969,	(const float)0.000885010,	(const float)0.000030518,
	(const float)-0.000213623,	(const float)-0.002578735,	(const float)-0.001693726,	(const float)-0.031005859,
	(const float)-0.040634155,	(const float)-0.143264771,	(const float)-0.376800537,	(const float)-1.111373901,
	(const float)0.765029907,	(const float)-0.016510010,	(const float)0.115921021,	(const float)-0.023422241,
	(const float)0.014022827,	(const float)-0.003463745,	(const float)0.000808716,	(const float)0.000030518,
	(const float)-0.000244141,	(const float)-0.002685547,	(const float)-0.002334595,	(const float)-0.031387329,
	(const float)-0.045837402,	(const float)-0.139450073,	(const float)-0.404083252,	(const float)-1.120223999,
	(const float)0.738372803,	(const float)-0.031082153,	(const float)0.110946655,	(const float)-0.025085449,
	(const float)0.012939453,	(const float)-0.003479004,	(const float)0.000747681,	(const float)0.000015259,
	(const float)-0.000259399,	(const float)-0.002792358,	(const float)-0.003005981,	(const float)-0.031661987,
	(const float)-0.051132202,	(const float)-0.134887695,	(const float)-0.431655884,	(const float)-1.127746582,
	(const float)0.711318970,	(const float)-0.044784546,	(const float)0.105819702,	(const float)-0.026535034,
	(const float)0.011886597,	(const float)-0.003479004,	(const float)0.000686646,	(const float)0.000015259,
	(const float)-0.000289917,	(const float)-0.002899170,	(const float)-0.003723145,	(const float)-0.031814575,
	(const float)-0.056533813,	(const float)-0.129577637,	(const float)-0.459472656,	(const float)-1.133926392,
	(const float)0.683914185,	(const float)-0.057617188,	(const float)0.100540161,	(const float)-0.027801514,
	(const float)0.010848999,	(const float)-0.003463745,	(const float)0.000625610,	(const float)0.000015259,
	(const float)-0.000320435,	(const float)-0.002990723,	(const float)-0.004486084,	(const float)-0.031845093,
	(const float)-0.061996460,	(const float)-0.123474121,	(const float)-0.487472534,	(const float)-1.138763428,
	(const float)0.656219482,	(const float)-0.069595337,	(const float)0.095169067,	(const float)-0.028884888,
	(const float)0.009841919,	(const float)-0.003433228,	(const float)0.000579834,	(const float)0.000015259,
	(const float)-0.000366211,	(const float)-0.003082275,	(const float)-0.005294800,	(const float)-0.031738281,
	(const float)-0.067520142,	(const float)-0.116577148,	(const float)-0.515609741,	(const float)-1.142211914,
	(const float)0.628295898,	(const float)-0.080688477,	(const float)0.089706421,	(const float)-0.029785156,
	(const float)0.008865356,	(const float)-0.003387451,	(const float)0.000534058,	(const float)0.000015259,
	(const float)-0.000396729,	(const float)-0.003173828,	(const float)-0.006118774,	(const float)-0.031478882,
	(const float)-0.073059082,	(const float)-0.108856201,	(const float)-0.543823242,	(const float)-1.144287109,
	(const float)0.600219727,	(const float)-0.090927124,	(const float)0.084182739,	(const float)-0.030517578,
	(const float)0.007919312,	(const float)-0.003326416,	(const float)0.000473022,	(const float)0.000015259
};
	
void InitSynthesisFilter (SynthesisFilter *filter, UINT channelnumber, float factor)
{
	register float *floatp, *floatp2;

	for (floatp = filter->v1 + 512, floatp2 = filter->v2 + 512; floatp > filter->v1; )
	 *--floatp = *--floatp2 = 0.0;

  
  for (floatp = filter->samples + 32; floatp > filter->samples; )
	 *--floatp = 0.0;

  filter->channel = channelnumber;
  filter->range_violations = filter->written_samples = 0;
  filter->actual_v = filter->v1;
  filter->actual_write_pos = 15;
  filter->scalefactor = factor;
}
void input_sample (SynthesisFilter *filter,float sample, UINT subbandnumber)
{
	filter->samples[subbandnumber] = sample;
}
 
float seconds_played (SynthesisFilter *filter,UINT frequency)
{
	return (float)filter->written_samples / (float)frequency;
}


void compute_new_v (SynthesisFilter* filter)
{
  float new_v[32];		// new V[0-15] and V[33-48] of Figure 3-A.2 in ISO DIS 11172-3
  register float *x1, *x2, tmp;
  float p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15;
  float pp0, pp1, pp2, pp3, pp4, pp5, pp6, pp7, pp8, pp9, pp10, pp11, pp12, pp13, pp14, pp15;

  // compute new values via a fast cosine transform:
  x1 = filter->samples;
  x2 = filter->samples + 31;
  p0  = *x1++ + *x2;   p1  = *x1++ + *--x2; p2  = *x1++ + *--x2; p3  = *x1++ + *--x2;
  p4  = *x1++ + *--x2; p5  = *x1++ + *--x2; p6  = *x1++ + *--x2; p7  = *x1++ + *--x2;
  p8  = *x1++ + *--x2; p9  = *x1++ + *--x2; p10 = *x1++ + *--x2; p11 = *x1++ + *--x2;
  p12 = *x1++ + *--x2; p13 = *x1++ + *--x2; p14 = *x1++ + *--x2; p15 = *x1   + *--x2;

  pp0  = p0 + p15; pp1 = p1 + p14; pp2 = p2 + p13; pp3 = p3 + p12;
  pp4  = p4 + p11; pp5 = p5 + p10; pp6 = p6 + p9;  pp7 = p7 + p8;
  pp8  = cos1_32  * (p0 - p15);
  pp9  = cos3_32  * (p1 - p14);
  pp10 = cos5_32  * (p2 - p13);
  pp11 = cos7_32  * (p3 - p12);
  pp12 = cos9_32  * (p4 - p11);
  pp13 = cos11_32 * (p5 - p10);
  pp14 = cos13_32 * (p6 - p9);
  pp15 = cos15_32 * (p7 - p8);

  p0  = pp0 + pp7; p1 = pp1 + pp6; p2 = pp2 + pp5; p3 = pp3 + pp4;
  p4  = cos1_16 * (pp0 - pp7);
  p5  = cos3_16 * (pp1 - pp6);
  p6  = cos5_16 * (pp2 - pp5);
  p7  = cos7_16 * (pp3 - pp4);
  p8  = pp8 + pp15; p9 = pp9 + pp14; p10 = pp10 + pp13; p11 = pp11 + pp12;
  p12 = cos1_16 * (pp8  - pp15);
  p13 = cos3_16 * (pp9  - pp14);
  p14 = cos5_16 * (pp10 - pp13);
  p15 = cos7_16 * (pp11 - pp12);

  pp0  = p0 + p3; pp1 = p1 + p2;
  pp2  = cos1_8 * (p0 - p3);
  pp3  = cos3_8 * (p1 - p2);
  pp4  = p4 + p7; pp5 = p5 + p6;
  pp6  = cos1_8 * (p4 - p7);
  pp7  = cos3_8 * (p5 - p6);
  pp8  = p8 + p11; pp9 = p9 + p10;
  pp10 = cos1_8 * (p8 - p11);
  pp11 = cos3_8 * (p9 - p10);
  pp12 = p12 + p15; pp13 = p13 + p14;
  pp14 = cos1_8 * (p12 - p15);
  pp15 = cos3_8 * (p13 - p14);

  p0 = pp0 + pp1;
  p1 = cos1_4 * (pp0 - pp1);
  p2 = pp2 + pp3;
  p3 = cos1_4 * (pp2 - pp3);
  p4 = pp4 + pp5;
  p5 = cos1_4 * (pp4 - pp5);
  p6 = pp6 + pp7;
  p7 = cos1_4 * (pp6 - pp7);
  p8  = pp8 + pp9;
  p9  = cos1_4 * (pp8 - pp9);
  p10 = pp10 + pp11;
  p11 = cos1_4 * (pp10 - pp11);
  p12 = pp12 + pp13;
  p13 = cos1_4 * (pp12 - pp13);
  p14 = pp14 + pp15;
  p15 = cos1_4 * (pp14 - pp15);

  tmp          = p6 + p7;
  new_v[36-17] = -(p5 + tmp);
  new_v[44-17] = -(p4 + tmp);
  tmp          = p11 + p15;
  new_v[10]    = tmp;
  new_v[6]     = p13 + tmp;
  tmp          = p14 + p15;
  new_v[46-17] = -(p8  + p12 + tmp);
  new_v[34-17] = -(p9  + p13 + tmp);
  tmp         += p10 + p11;
  new_v[38-17] = -(p13 + tmp);
  new_v[42-17] = -(p12 + tmp);
  new_v[2]     = p9 + p13 + p15;
  new_v[4]     = p5 + p7;
  new_v[48-17] = -p0;
  new_v[0]     = p1;
  new_v[8]     = p3;
  new_v[12]    = p7;
  new_v[14]    = p15;
  new_v[40-17] = -(p2  + p3);

  x1 = filter->samples;
  x2 = filter->samples + 31;
  p0  = cos1_64  * (*x1++ - *x2);   p1  = cos3_64  * (*x1++ - *--x2);
  p2  = cos5_64  * (*x1++ - *--x2); p3  = cos7_64  * (*x1++ - *--x2);
  p4  = cos9_64  * (*x1++ - *--x2); p5  = cos11_64 * (*x1++ - *--x2);
  p6  = cos13_64 * (*x1++ - *--x2); p7  = cos15_64 * (*x1++ - *--x2);
  p8  = cos17_64 * (*x1++ - *--x2); p9  = cos19_64 * (*x1++ - *--x2);
  p10 = cos21_64 * (*x1++ - *--x2); p11 = cos23_64 * (*x1++ - *--x2);
  p12 = cos25_64 * (*x1++ - *--x2); p13 = cos27_64 * (*x1++ - *--x2);
  p14 = cos29_64 * (*x1++ - *--x2); p15 = cos31_64 * (*x1   - *--x2);

  pp0  = p0 + p15; pp1 = p1 + p14; pp2 = p2 + p13; pp3 = p3 + p12;
  pp4  = p4 + p11; pp5 = p5 + p10; pp6 = p6 + p9;  pp7 = p7 + p8;
  pp8  = cos1_32  * (p0 - p15);
  pp9  = cos3_32  * (p1 - p14);
  pp10 = cos5_32  * (p2 - p13);
  pp11 = cos7_32  * (p3 - p12);
  pp12 = cos9_32  * (p4 - p11);
  pp13 = cos11_32 * (p5 - p10);
  pp14 = cos13_32 * (p6 - p9);
  pp15 = cos15_32 * (p7 - p8);

  p0  = pp0 + pp7; p1 = pp1 + pp6; p2 = pp2 + pp5; p3 = pp3 + pp4;
  p4  = cos1_16 * (pp0 - pp7);
  p5  = cos3_16 * (pp1 - pp6);
  p6  = cos5_16 * (pp2 - pp5);
  p7  = cos7_16 * (pp3 - pp4);
  p8  = pp8  + pp15; p9 = pp9  + pp14; p10 = pp10 + pp13; p11 = pp11 + pp12;
  p12 = cos1_16 * (pp8  - pp15);
  p13 = cos3_16 * (pp9  - pp14);
  p14 = cos5_16 * (pp10 - pp13);
  p15 = cos7_16 * (pp11 - pp12);

  pp0  = p0 + p3; pp1 = p1 + p2;
  pp2  = cos1_8 * (p0 - p3);
  pp3  = cos3_8 * (p1 - p2);
  pp4  = p4 + p7; pp5 = p5 + p6;
  pp6  = cos1_8 * (p4 - p7);
  pp7  = cos3_8 * (p5 - p6);
  pp8  = p8 + p11; pp9 = p9 + p10;
  pp10 = cos1_8 * (p8 - p11);
  pp11 = cos3_8 * (p9 - p10);
  pp12 = p12 + p15; pp13 = p13 + p14;
  pp14 = cos1_8 * (p12 - p15);
  pp15 = cos3_8 * (p13 - p14);

  p0 = pp0 + pp1;
  p1 = cos1_4 * (pp0 - pp1);
  p2 = pp2 + pp3;
  p3 = cos1_4 * (pp2 - pp3);
  p4 = pp4 + pp5;
  p5 = cos1_4 * (pp4 - pp5);
  p6 = pp6 + pp7;
  p7 = cos1_4 * (pp6 - pp7);
  p8  = pp8 + pp9;
  p9  = cos1_4 * (pp8 - pp9);
  p10 = pp10 + pp11;
  p11 = cos1_4 * (pp10 - pp11);
  p12 = pp12 + pp13;
  p13 = cos1_4 * (pp12 - pp13);
  p14 = pp14 + pp15;
  p15 = cos1_4 * (pp14 - pp15);

  tmp          = p13 + p15;
  new_v[1]     = p1 + p9 + tmp;
  new_v[5]     = p5 + p7 + p11 + tmp;
  tmp         += p9;
  new_v[33-17] = -(p1 + p14 + tmp);
  tmp         += p5 + p7;
  new_v[3]     = tmp;
  new_v[35-17] = -(p6 + p14 + tmp);
  tmp          = p10 + p11 + p12 + p13 + p14 + p15;
  new_v[39-17] = -(p2 + p3 + tmp - p12);
  new_v[43-17] = -(p4 + p6 + p7 + tmp - p13);
  new_v[37-17] = -(p5 + p6 + p7 + tmp - p12);
  new_v[41-17] = -(p2 + p3 + tmp - p13);
  tmp          = p8 + p12 + p14 + p15;
  new_v[47-17] = -(p0 + tmp);
  new_v[45-17] = -(p4 + p6 + p7 + tmp);
  tmp          = p11 + p15;
  new_v[11]    = p7  + tmp;
  tmp         += p3;
  new_v[9]     = tmp;
  new_v[7]     = p13 + tmp;
  new_v[13]    = p7 + p15;
  new_v[15]    = p15;

  // insert V[0-15] (== new_v[0-15]) into actual v:
  x1 = new_v;
  x2 = filter->actual_v + filter->actual_write_pos;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1;   x2 += 16;
  // V[16] is always 0.0:
  *x2 = 0.0; x2 += 16;
  // insert V[17-31] (== -new_v[15-1]) into actual v:
  *x2 = -*x1;   x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16;
  *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16;
  *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16;
  *x2 = -*--x1; x2 += 16; *x2 = -*--x1; x2 += 16; *x2 = -*--x1;

  // insert V[32] (== -new_v[0]) into other v:
  x2 = (filter->actual_v == filter->v1 ? filter->v2 : filter->v1) + filter->actual_write_pos;
  *x2 = -*--x1; x2 += 16;
  // insert V[33-48] (== new_v[16-31]) into other v:
  x1 = new_v + 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16;
  *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1++; x2 += 16; *x2 = *x1;   x2 += 16;
  // insert V[49-63] (== new_v[30-16]) into other v:
  *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16;
  *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16;
  *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16;
  *x2 = *--x1; x2 += 16; *x2 = *--x1; x2 += 16; *x2 = *--x1;
}


void compute_pcm_samples (int ch, SynthesisFilter* filter, SOUNDBUF *soundbuf)
{
  UINT i;
  register float floatreg, *vp;
  register const float *dp;
  int pcm_sample;

  i = 0;
  dp = d;

  switch (filter->actual_write_pos)
  {
	 case 0:
		 vp = filter->actual_v;
		 for (; i < 32; ++i)
		 {
			 floatreg = *vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg;    */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 1:
		 vp = filter->actual_v + 1;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 2:
		 vp = filter->actual_v + 2;
		 for (; i < 32; ++i)
		 {
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
				 ++filter->range_violations;
				 /*	  if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 3:
		 vp = filter->actual_v + 3;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 4:
		 vp = filter->actual_v + 4;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 5:
		 vp = filter->actual_v + 5;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg;   */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg;   */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 6:
		 vp = filter->actual_v + 6;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 7:
		 vp = filter->actual_v + 7;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg;   */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg;   */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 8:
		 vp = filter->actual_v + 8;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 9:
		 vp = filter->actual_v + 9;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
				 ++filter->range_violations;
				 /*	  if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 10:
		 vp = filter->actual_v + 10;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 11:
		 vp = filter->actual_v + 11;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 12:
		 vp = filter->actual_v + 12;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 13:
		 vp = filter->actual_v + 13;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;   floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 14:
		 vp = filter->actual_v + 14;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++;
			 vp += 15;
			 floatreg += *vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 15;
		 }
		 break;
	 case 15:
		 vp = filter->actual_v + 15;
		 for (; i < 32; ++i)
		 {
			 
			 floatreg = *vp * *dp++;    floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 floatreg += *--vp * *dp++; floatreg += *--vp * *dp++;
			 pcm_sample = (int)(floatreg * filter->scalefactor);
			 if (pcm_sample > 32767)
			 {
			 /*	  ++range_violations;
			 if (floatreg > max_violation)
				 max_violation = floatreg; */
				 pcm_sample = 32767;
			 }
			 else if (pcm_sample < -32768)
			 {
			 /*	  ++range_violations;
			 if (-floatreg > max_violation)
				 max_violation = -floatreg; */
				 pcm_sample = -32768;
			 }
			 
			 AppendData(ch, soundbuf, pcm_sample );
			 vp += 31;
		 }
  }
  
}

void calculate_pcm_samples (int ch, SynthesisFilter *filter, SOUNDBUF *sbuf)
{
	compute_new_v (filter) ;
	compute_pcm_samples (ch, filter, sbuf);
	
	filter->written_samples += 32;
	if (filter->actual_write_pos < 15)
		++filter->actual_write_pos;
	else
		filter->actual_write_pos = 0;
	filter->actual_v = (filter->actual_v == filter->v1 ? filter->v2 : filter->v1);
	
	// initialize samples[]:
	for (register float *floatp = filter->samples + 32; floatp > filter->samples; )
		*--floatp = 0.0;
}
