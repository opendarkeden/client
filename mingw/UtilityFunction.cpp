//-----------------------------------------------------------------------------
// UtilityFunction.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "DXLib.h"

#include "UtilityFunction.h"


bool LoadJPG(LPCTSTR lpszFileName, int &width, int &height, int &bpp, unsigned char** p_data);
bool SaveJPG(LPCTSTR lpszFileName, int &width, int &height, int &bpp, unsigned char* p_data);


//-----------------------------------------------------------------------------
// RemoveStringSpace
//-----------------------------------------------------------------------------
// string 앞 뒤의 공백을 제거한다.
//-----------------------------------------------------------------------------
void
RemoveStringSpace(char*& str)
{
	// 앞쪽 공백 제거
	int bExistChar = 0;
	while (*str != '\0')
	{
		if (*str == ' ')
		{
			str++;
		}
		else
		{
			bExistChar = 1;
			break;
		}
	}

	// 뒤쪽 공백 제거 
	if (bExistChar)
	{
		char* strTemp = str;
		strTemp += strlen(str)-1;

		while (*strTemp == ' ')
		{
			strTemp--;
		}

		*(strTemp+1) = '\0';
	}
}

//-----------------------------------------------------------------------------
// SSN Check (strSSN1, strSSN2)
//-----------------------------------------------------------------------------
// 주민등록번호 체크..
// strSSN1-strSSN2
// 
// 정상적인 주민등록번호이면	return 1
//					아니면		return 0
//-----------------------------------------------------------------------------
int 
IsValidSSN(const char* strSSN1, const char* strSSN2)
{
	if (strSSN1==0 || strSSN2==0)
	{
		return 0;
	}

	//----------------------------------------------------------
	// SSN1 의 길이 체크. 6자여야 한다.
	//----------------------------------------------------------
	int lenSSN1 = strlen(strSSN1);

	if (lenSSN1 != 6)
	{
		return 0;
	}

	//----------------------------------------------------------
	// SSN2의 길이 체크. 7자여야 한다.
	//----------------------------------------------------------
	int lenSSN2 = strlen(strSSN2);

	if (lenSSN2 != 7)
	{
		return 0;
	}

	//----------------------------------------------------------
	// 각 자리의 숫자를 읽는다.
	//----------------------------------------------------------
	const int chZero = '0';

	int n1_0 = strSSN1[0] - chZero;
	int n1_1 = strSSN1[1] - chZero;
	int n1_2 = strSSN1[2] - chZero;
	int n1_3 = strSSN1[3] - chZero;
	int n1_4 = strSSN1[4] - chZero;
	int n1_5 = strSSN1[5] - chZero;

	int n2_0 = strSSN2[0] - chZero;
	int n2_1 = strSSN2[1] - chZero;
	int n2_2 = strSSN2[2] - chZero;
	int n2_3 = strSSN2[3] - chZero;
	int n2_4 = strSSN2[4] - chZero;
	int n2_5 = strSSN2[5] - chZero;
	int n2_6 = strSSN2[6] - chZero;		// check number

	//----------------------------------------------------------
	// check할려는 숫자를 계산한다.
	//----------------------------------------------------------
	int sum = n1_0*2 + n1_1*3 + n1_2*4 + n1_3*5 + n1_4*6 + n1_5*7 
			+ n2_0*8 + n2_1*9 + n2_2*2 + n2_3*3 + n2_4*4 + n2_5*5;

    int parity = sum % 11;

	//----------------------------------------------------------
	// 다른 경우
	//----------------------------------------------------------
    if ( ((11-n2_6)) % 10 != (parity % 10) ) 
	{ 
		return 0; 
	}

    return 1;
}

//-----------------------------------------------------------------------------
// Is Valid ID
//-----------------------------------------------------------------------------
// 4~10글자 공백이 있으면 안된다.
// 다 한글이거나
// 다 영어야 할까??
/*
	char temp[3];
	strncpy(temp, str, 2);
	temp[2] = NULL;

	if (strcmp(temp, "가") >= 0
		&& strcmp(temp, "힝") <= 0)
	{
		// 제대로된 한글
		str += 2;					
		continue;	
	}
	else
	{
		// 2바이트 문자
		str += 2;
		continue;
	}
*/
//-----------------------------------------------------------------------------
int 
IsValidID(const char* strID, const char* strPermit)
{
	const int minLength = 4;
	const int maxLength = 10;

	// 어떠한 경우이든 허용되는 문자들
	//const char* strPermit = "_-";

	int len = 0;

	char strtempID[128];

	strcpy( strtempID, strID );

	char* str = strtempID;

	//--------------------------------------------------------
	// 공백이 들어가면 안된다.
	// 길이도 알아낸다. *_*;
	//--------------------------------------------------------
	while (*str != '\0')
	{
		if (*str == ' ')
		{
			return 0;
		}

		len++;
		str++;
	}

	//--------------------------------------------------------
	// 길이가 잘못된 경우
	//--------------------------------------------------------
	if (len<minLength || len>maxLength)
	{
		return 0;
	}

	//--------------------------------------------------------
	// 첫글자가 숫자이면 안된다.
	//--------------------------------------------------------
	if (strtempID[0]>='0' && strtempID[0]<='9')
	{
		return 0;
	}

	//--------------------------------------------------------
	// 한글을 제외한 특수문자가 들어가면 안된다.
	//--------------------------------------------------------
	int bExistHangul = 0;
	int bExistEnglish = 0;

	str = strtempID;
	while (*str != '\0')
	{
		char ch = *str;
		//--------------------------------------------------------
		// 첫 bit가 '1'이면 한글..일까?
		//--------------------------------------------------------
		if (ch & 0x80)
		{
			// 한글이므로 두 byte 다음 거 체크
			str++;

			if (*str=='\0')
			{
				// 다음게 없으면 잘못된 ID이다.
				return 0;				
			}

			//--------------------------------------------------------
			// 자음, 모음만 있는 경우라면 잘못된 아이디다.
			//--------------------------------------------------------
			const char* badKor = "ㅂㅃㅈㅉㄷㄸㄱㄲㅅㅆㅛㅕㅑㅐㅒㅔㅖㅁㄴㅇㄹㅎㅗㅓㅏㅣㅋㅌㅊㅍㅠㅜㅡㅙㅝㅟㅢ";
			
			char strKor[3] = { ch, *str };
			
			char* findPtr = strstr(badKor, strKor);

			if (findPtr!=NULL)
			{
				if (!((findPtr - badKor) & 0x01))	// 짝수라면..
				{					
					return 0;
				}
			}

			str++;

			bExistHangul = 1;
		}
		//--------------------------------------------------------
		// 아니면..
		//--------------------------------------------------------
		else
		{
			//--------------------------------------------------------
			// 영어 대소문자..
			//--------------------------------------------------------
			if (ch>='a' && ch<='z' 
				|| ch>='A' && ch<='Z')
			{				
				bExistEnglish = 1;
			}
			//--------------------------------------------------------
			// 숫자거나 
			// 허용된 문자인 경우는 괜찮다..
			//--------------------------------------------------------
			else if (ch>='0' && ch<='9'
					|| strPermit!=NULL && strchr(strPermit, ch)!='\0')
			{				
			}
			//--------------------------------------------------------
			// 이상한 문자 쓰면 안된다.				
			//--------------------------------------------------------
			else
			{
				return 0;
			}

			str++;

		}
	}

	//--------------------------------------------------------
	// 한글을 사용한 경우에는 영어를 사용할 수 없다.
	//--------------------------------------------------------
	if (bExistHangul && bExistEnglish)
	{
		return 0;
	}

	//--------------------------------------------------------
	// 정상적인 ID
	//--------------------------------------------------------
	return 1;
}

//-----------------------------------------------------------------------------
// Is Valid Password
//-----------------------------------------------------------------------------
int 
IsValidPassword(const char* strPWD)
{
	//--------------------------------------------------
	// 허용안되는 특수문자를 사용하면 안된다.
	//--------------------------------------------------
	if (strchr(strPWD, '\\')!=NULL
		|| strchr(strPWD, '\'')!=NULL)
	{
		return 0;
	}

	//--------------------------------------------------
	// 숫자만 사용하면 안된다.
	//--------------------------------------------------
	// 흑흑.. 이미 숫자가 입력된 사람들 때메.. T_T;;
	/*
	char* str = strPWD;
	
	char ch;

	bool AllNumber = TRUE;

	while (ch=*str++, ch)
	{
		if (ch<'0' || ch>'9')
		{
			AllNumber = FALSE;
			break;
		}
	}

	if (AllNumber)	// 전부 숫자인 경우..
	{
		return 0;
	}
	*/

	return 1;
}

//-----------------------------------------------------------------------------
// LoadImageToSurface
//-----------------------------------------------------------------------------
// *.bmp, *.jpg만 읽는다.
//-----------------------------------------------------------------------------
bool	
LoadImageToSurface(const char* pFilename, CDirectDrawSurface& surface)
{
	if (pFilename==NULL)
	{
		return false;
	}

	int fileLen = strlen(pFilename);

	// file이름이 넘 짧은 경우... strlen("이름.bmp")==8
	if (fileLen < 8)
	{
		return false;
	}

	// file이름이 이상한 경우
	char checkStr[10];
	strcpy(checkStr, (pFilename+fileLen-4));
	strcpy(checkStr, _strlwr(checkStr));

	bool bBmp = false;
	bool bJpg = false;

	// 확장자 체크를 위해 lowercase
	for(int kkk = 0; kkk < strlen(checkStr); kkk++)
		if(checkStr[kkk] >= 'A' && checkStr[kkk] <= 'Z')
			checkStr[kkk] += 'a' - 'A';

	// 확장자 체크..
	if(!strncmp(".bmp", checkStr, 4))
	{
		bBmp = true;
	}
	else if(!strncmp(".jpg", checkStr, 4)
			|| !strncmp("jpeg", checkStr, 4))
	{
		bJpg = true;
	}
	else 
	{
		return false;
	}

	bool bOK = false;

	if (bBmp)
	{
		bOK = surface.InitFromBMP(pFilename, DDSCAPS_SYSTEMMEMORY);
	}
	else if (bJpg)
	{
		int width = 0, height = 0, bpp = 0;
		unsigned char* p_data = NULL;
		
		if (!LoadJPG(pFilename, width, height, bpp, &p_data))
		{
			return false;
		}

		int pitch = width*bpp;

		int green_shift = 0;

		if (width > 0 && height > 0 && bpp > 0)
		{
			if (surface.InitOffsurface(width, height, DDSCAPS_SYSTEMMEMORY))
			{
				if (surface.Lock())
				{
					WORD *pSurface = (WORD *)surface.GetSurfacePointer();
					unsigned char *pData = p_data, *pDataTemp;
					WORD *pSurfaceTemp;

					int surfacePitch = surface.GetSurfacePitch();

					if (pSurface)
					{
						if (bpp == 1)
						{
							for (register int y = 0; y < height; y++)
							{
								pDataTemp = pData;
								pSurfaceTemp = pSurface;								

								for (register int x = 0; x < width; x++)
								{
									BYTE temp_data = *pDataTemp++;	//p_data[y*(pitch)+x];
									BYTE r = temp_data>>3;
									BYTE g = r;
									BYTE b = r;

									*pSurfaceTemp++ = CDirectDraw::Color(r, g, b);
								}

								pData = pData + pitch;
								pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
							}
						}
						else if (bpp == 3)
						{
							for (register int y = 0; y < height; y++)
							{
								pDataTemp = pData;
								pSurfaceTemp = pSurface;	

								for (register int x = 0; x < width; x++)
								{
									//char *temp_data = &p_data[y*pitch+x*bpp];
									BYTE r = *(pDataTemp+2) >> 3;		//temp_data[2]>>3;
									BYTE g = *(pDataTemp+1) >> 3;	//temp_data[1]>>3;
									BYTE b = *pDataTemp >> 3;	//temp_data[0]>>3;

									pDataTemp += bpp;

									*pSurfaceTemp++ = CDirectDraw::Color(r, g, b);
								}

								pData = pData + pitch;
								pSurface = (WORD*)((BYTE*)pSurface + surfacePitch);
							}
						}

						bOK = true;
					}

					surface.Unlock();
				}
			}			
		}
		
		if (p_data!=NULL)
		{
			delete [] p_data;
		}
	}

	return bOK;
}

//-----------------------------------------------------------------------------
// SaveSurfaceToImage
//-----------------------------------------------------------------------------
// *.bmp, *.jpg만 쓴다.
//-----------------------------------------------------------------------------
bool	
SaveSurfaceToImage(const char* pFilename, CDirectDrawSurface& surface)
{
	if (pFilename==NULL)
	{
		return false;
	}

	int fileLen = strlen(pFilename);

	// file이름이 넘 짧은 경우... strlen("이름.bmp")==8
	if (fileLen < 8)
	{
		return false;
	}

	// file이름이 이상한 경우
	char checkStr[10];
	strcpy(checkStr, (pFilename+fileLen-4));
	strcpy(checkStr, _strlwr(checkStr));

	bool bBmp = false;
	bool bJpg = false;

	// 확장자 체크를 위해 lowercase
	for(int kkk = 0; kkk < strlen(checkStr); kkk++)
		if(checkStr[kkk] >= 'A' && checkStr[kkk] <= 'Z')
			checkStr[kkk] += 'a' - 'A';

	// 확장자 체크..
	if(!strncmp(".bmp", checkStr, 4))
	{
		bBmp = true;
	}
	else if(!strncmp(".jpg", checkStr, 4)
			|| !strncmp("jpeg", checkStr, 4))
	{
		bJpg = true;
	}
	else 
	{
		return false;
	}

	bool bOK = false;

	if (bBmp)
	{
		bOK = surface.SaveToBMP(pFilename);
	}
	else if (bJpg)
	{
		int width = surface.GetWidth(), height = surface.GetHeight(), bpp = surface.Get_BPP();
		int out_bpp = 24;
		if(surface.Lock())
		{
			int surfacePitch = surface.GetSurfacePitch();

			WORD *pSurfacePoint = (WORD *)surface.GetSurfacePointer();
			BYTE* p_data = new BYTE[width*height*out_bpp/8];
			BYTE* p_dataTemp = p_data;

			for (register int y = 0; y < height; y++)
			{
				WORD *pSurfacePointTemp = pSurfacePoint;

				for (register int x = 0; x < width; x++)
				{
					WORD color = *pSurfacePointTemp;
					p_dataTemp[0] = CDirectDraw::Blue(*pSurfacePointTemp)<<3;
					p_dataTemp[1] = CDirectDraw::Green(*pSurfacePointTemp)<<3;
					p_dataTemp[2] = CDirectDraw::Red(*pSurfacePointTemp)<<3;
				
					pSurfacePointTemp++;
					p_dataTemp += 3;
				}
				
				pSurfacePoint += surfacePitch/2;
			}
			surface.Unlock();

			if (!SaveJPG(pFilename, width, height, out_bpp, p_data))
			{
				delete []p_data;
				return false;
			}
			bOK = true;
			delete []p_data;

		}

	}

	return bOK;
}

/////////////////////////////////////////////////////////////////////////////
// CBaseImage

extern "C" {
#include "jpegLib\jpeglib.h"
}

/*
 * Here's the routine that will replace the standard error_exit method:
 */

//add by zdj
#include <setjmp.h>

struct ima_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct ima_error_mgr * ima_error_ptr;

METHODDEF(void)
ima_jpeg_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  ima_error_ptr myerr = (ima_error_ptr) cinfo->err;

  char buffer[JMSG_LENGTH_MAX];

  /* Create the message */
  myerr->pub.format_message (cinfo, buffer);

  /* Send it to stderr, adding a newline */
//        AfxMessageBox(buffer);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

bool LoadJPG(LPCTSTR lpszFileName, int &width, int &height, int &bpp, unsigned char** p_data)
{

	BOOL bGray = FALSE;
  /* This struct contains the JPEG decompression parameters and pointers to
	* working space (which is allocated as needed by the JPEG library).
	*/
  struct jpeg_decompress_struct cinfo;
  /* We use our private extension JPEG error handler. */

  struct ima_error_mgr jerr;
//  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * infile;		/* source file */
  JSAMPARRAY buffer;		/* Output row buffer */
  int row_stride;		/* physical row width in output buffer */

  /* In this example we want to open the input file before doing anything else,
	* so that the setjmp() error recovery below can assume the file is open.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to read binary files.
	*/

  if ((infile = fopen(lpszFileName, "rb")) == NULL) {
	 //fprintf(stderr, "can't open %s\n", filename);
	 return 0;
  }

  /* Step 1: allocate and initialize JPEG decompression object */
  /* We set up the normal JPEG error routines, then override error_exit. */
  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = ima_jpeg_error_exit;

  /* Establish the setjmp return context for my_error_exit to use. */
  if (setjmp(jerr.setjmp_buffer)) {
	 /* If we get here, the JPEG code has signaled an error.
	  * We need to clean up the JPEG object, close the input file, and return.
	  */
	 jpeg_destroy_decompress(&cinfo);
	 fclose(infile);
	 return 0;
  }
  /* Now we can initialize the JPEG decompression object. */
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */
  jpeg_stdio_src(&cinfo, infile);

  /* Step 3: read file parameters with jpeg_read_header() */
  (void) jpeg_read_header(&cinfo, TRUE);

  /* Step 4: set parameters for decompression */
//  printf("info %d %d %d CS %d ", cinfo.image_width, cinfo.image_height, cinfo.output_components, cinfo.jpeg_color_space);
  if (cinfo.jpeg_color_space!=JCS_GRAYSCALE) {
	cinfo.quantize_colors = TRUE;
	cinfo.desired_number_of_colors = 128;
  }
  /* Step 5: Start decompressor */
  jpeg_start_decompress(&cinfo);

  /* We may need to do some setup of our own at this point before reading
	* the data.  After jpeg_start_decompress() we have the correct scaled
	* output image dimensions available, as well as the output colormap
	* if we asked for color quantization.
	*/

	width = cinfo.image_width;
	height = cinfo.image_height;
	bpp = 0;

//  char *p_data = NULL;

	if(width <= 0 || height <= 0)
		return 0;

  if (cinfo.jpeg_color_space==JCS_GRAYSCALE)
  {
	  bGray = TRUE;
	  bpp = 1;
	  *p_data = new unsigned char [width*height*bpp];
//	  CreateImage(cinfo.image_width, cinfo.image_height, 8);
//	  Create(cinfo.image_width, cinfo.image_height, 8);
  }
  else
  {
	  bpp = 3;
	  *p_data = new unsigned char [width*height*bpp];
  }
	  //CreateImage(cinfo.image_width, cinfo.image_height, 24);
//	  Create(cinfo.image_width, cinfo.image_height, 24);

  /* JSAMPLEs per row in output buffer */
  row_stride = cinfo.output_width * cinfo.output_components;
//  byte* buf2 = new byte[row_stride];
//  printf("NCMPS cmp [%d %d %d]", cinfo.output_components, cinfo.actual_number_of_colors,row_stride);

  /* Make a one-row-high sample array that will go away when done with image */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */

  /* Here we use the library's state variable cinfo.output_scanline as the
  * loop counter, so that we don't have to keep track ourselves.
  */
  int line, col;

   if(bGray)
  {
	  line = 0;
	  while (cinfo.output_scanline < cinfo.output_height) 
	  {
		 (void) jpeg_read_scanlines(&cinfo, buffer, 1);
		 /* Assume put_scanline_someplace wants a pointer and sample count. */
//		 memcpy(ptr[line++], buffer[0], row_stride);
		 memcpy(*p_data+line*row_stride, buffer[0], row_stride);
		 line++;
	  }
  }
  else 
  {
	  line = 0;

	  while (cinfo.output_scanline < cinfo.output_height) 
	  {
		  (void) jpeg_read_scanlines(&cinfo, buffer, 1);
		  /* Assume put_scanline_someplace wants a pointer and sample count. */
		  for(col=0 ; col<row_stride ; col++) 
		  {
			  (*p_data)[line*row_stride*bpp+col*3+2] = cinfo.colormap[0][buffer[0][col]];
			  (*p_data)[line*row_stride*bpp+col*3+1] = cinfo.colormap[1][buffer[0][col]];
			  (*p_data)[line*row_stride*bpp+col*3+0] = cinfo.colormap[2][buffer[0][col]];
		  }
		  line++;
	  }
  }


  /* Step 7: Finish decompression */
  (void) jpeg_finish_decompress(&cinfo);
  /* We can ignore the return value since suspension is not possible
	* with the stdio data source.
	*/

  /* Step 8: Release JPEG decompression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  /* After finish_decompress, we can close the input file.
	* Here we postpone it until after no more JPEG errors are possible,
	* so as to simplify the setjmp error logic above.  (Actually, I don't
	* think that jpeg_destroy can do an error exit, but why assume anything...)
	*/
  fclose(infile);
  /* At this point you may want to check to see whether any corrupt-data
	* warnings occurred (test whether jerr.pub.num_warnings is nonzero).
	*/


//  delete p_data;
  /* And we're done! */
  return 1;
}

bool SaveJPG(LPCTSTR lpszFileName, int &width, int &height, int &bpp, unsigned char* p_data)
{
	BOOL bGray = FALSE;
	int i;

  struct jpeg_compress_struct cinfo;
  /* This struct represents a JPEG error handler.  It is declared separately
	* because applications often want to supply a specialized error handler
	* (see the second half of this file for an example).  But here we just
	* take the easy way out and use the standard error handler, which will
	* print a message on stderr and call exit() if compression fails.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  int row_stride;		/* physical row width in image buffer */
  JSAMPARRAY buffer;		/* Output row buffer */

  /* Step 1: allocate and initialize JPEG compression object */

  /* We have to set up the error handler first, in case the initialization
	* step fails.  (Unlikely, but it could happen if you are out of memory.)
	* This routine fills in the contents of struct jerr, and returns jerr's
	* address which we place into the link field in cinfo.
	*/
  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Step 2: specify data destination (eg, a file) */
  /* Note: steps 2 and 3 can be done in either order. */

  /* Here we use the library-supplied code to send compressed data to a
	* stdio stream.  You can also write your own code to do something else.
	* VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
	* requires it in order to write binary files.
	*/
  if ((outfile = fopen(lpszFileName, "wb")) == NULL) {
//	 fprintf(stderr, "can't open %s\n", filename);
	 return FALSE;
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* Step 3: set parameters for compression */

  /* First we supply a description of the input image.
	* Four fields of the cinfo struct must be filled in:
	*/
  cinfo.image_width = width; 	// image width and height, in pixels
  cinfo.image_height = height;
  if(bpp == 8)
  {
	  cinfo.input_components = 1; 	// # of color components per pixel
	  cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
	  bGray = TRUE;
  }
  else
  {
	  cinfo.input_components = 3; 	// # of color components per pixel
	  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */
  }

//  printf("info %d %d %d %d ", cinfo.image_width, cinfo.image_height, cinfo.input_components, cinfo.in_color_space);
  /* Now use the library's routine to set default compression parameters.
	* (You must set at least cinfo.in_color_space before calling this,
	* since the defaults depend on the source color space.)
	*/
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
	* Here we just illustrate the use of quality (quantization table) scaling:
	*/
  //jpeg_set_quality(&cinfo, 30, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
	* Pass TRUE unless you are very sure of what you're doing.
	*/
//  puts("begining");
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	* To keep things simple, we pass one scanline per call; you can pass
	* more if you wish, though.
	*/
  row_stride = width*cinfo.input_components;	/* JSAMPLEs per row in image_buffer */

  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	int line = 0;
	if (bGray) 
	{
		while (cinfo.next_scanline < cinfo.image_height) 
		{
			memcpy(buffer[0], p_data+line*row_stride, row_stride);
			(void) jpeg_write_scanlines(&cinfo, buffer, 1);
		}
	}
	else
	{
		while (cinfo.next_scanline < cinfo.image_height) 
		{
			for(i=0 ; i<row_stride ; i+=3)
			{
				buffer[0][i] = p_data[line*row_stride+i+2];
				buffer[0][i+1] = p_data[line*row_stride+i+1];
				buffer[0][i+2] = p_data[line*row_stride+i+0];
			}
			line++;
			(void) jpeg_write_scanlines(&cinfo, buffer, 1);
		}
	 
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */
  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

	/* And we're done! */
  return TRUE;
}


//-----------------------------------------------------------------------------
// Get DiskFreeSpace
//-----------------------------------------------------------------------------
// Drive가 NULL이면 지정을 안하면 현재 드라이브다.
//-----------------------------------------------------------------------------
unsigned long
GetDiskFreeSpace(const char* pDrive)
{	
	DWORD dwSectorsPerCluster;
	DWORD dwBytesPerSector;
	DWORD dwNumberOfFreeClusters;
	DWORD dwTotalNumberOfClusters;

	GetDiskFreeSpace(pDrive,
						&dwSectorsPerCluster, 
						&dwBytesPerSector, 
						&dwNumberOfFreeClusters, 
						&dwTotalNumberOfClusters);

	DWORD bytesPerCluster = dwSectorsPerCluster * dwBytesPerSector;
	DWORD freeBytes = dwNumberOfFreeClusters * bytesPerCluster;
	//DWORD totalBytes = dwTotalNumberOfClusters * bytesPerCluster;

	return freeBytes;
}