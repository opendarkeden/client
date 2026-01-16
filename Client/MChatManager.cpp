//----------------------------------------------------------------------
// MChatManager.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include "MChatManager.h"


#ifdef __GAME_CLIENT__
	#include "MGameStringTable.h"
	#include "UserOption.h"
#endif


//----------------------------------------------------------------------
// Global
//----------------------------------------------------------------------
MChatManager*		g_pChatManager = NULL;

//----------------------------------------------------------------------
// Static
//----------------------------------------------------------------------
char MChatManager::s_MaskString[256] = //"^^; -_-; !_!; o_O; *_*; m_m; u_u; p_q; =_=; -_+; $_$; v_v; Y_Y; o_o; O_O; w_w; #_#; ._.; n_n; &_&; @_@; 0_0; _-_; +_+; +_=; ~_~;";
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
char MChatManager::s_MaskString2[256] = 
		"#&*%!$#%&*@!%&$&#*&$@#*!%$&@#&*%!$#%&*@!%&$&#*&$@&#!%$*&%*#@#*!%$&@#&*%!$#%&*@!%&$&#*&$@#*!%$&@#&*!$#%&*@!%&$&#*&$@#*!%$&@#&*%!$#%&!$#%&*@!%&$&#*&$@#*!%$&@#&*%!$#%&*";
		//".....................................................................................................................................................................";

//----------------------------------------------------------------------
//
// constructor / destructor
// 
//----------------------------------------------------------------------
MChatManager::MChatManager()
{
	m_bIgnoreMode = false;
}

MChatManager::~MChatManager()
{
}

//----------------------------------------------------------------------
//
// member functions
//
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Save To File
//----------------------------------------------------------------------
void				
MChatManager::SaveToFile(const char* filename)
{
	std::ofstream file(filename, ios::binary);

	m_mapCurseEng.SaveToFile( file );
	m_mapCurseKor1.SaveToFile( file );
	m_mapCurseKor2.SaveToFile( file );
	m_mapCurseKor3.SaveToFile( file );
	m_mapCurseKor4.SaveToFile( file );
	m_mapID.SaveToFile( file );
	
	file.close();
}

//----------------------------------------------------------------------
// Load From File
//----------------------------------------------------------------------
void				
MChatManager::LoadFromFile(const char* filename)
{
	std::ifstream file(filename, ios::binary);

	m_mapCurseEng.LoadFromFile( file );
	m_mapCurseKor1.LoadFromFile( file );
	m_mapCurseKor2.LoadFromFile( file );
	m_mapCurseKor3.LoadFromFile( file );
	m_mapCurseKor4.LoadFromFile( file );
	m_mapID.LoadFromFile( file );

	file.close();
}

//----------------------------------------------------------------------
// Load From File Curse
//----------------------------------------------------------------------
// text file에서 욕 읽기
//----------------------------------------------------------------------
void				
MChatManager::LoadFromFileCurse(const char* filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		return;
	}

	char str[256];

	//-----------------------------------------------------
	// text file에서 죽죽~~ 읽어들인다.
	//-----------------------------------------------------
	while (!file.eof())
	{
		file >> str;

		bool bEng = true;
		bool bKor = true;

		char* strTemp = str;
		char ch;

		//-----------------------------------------------------
		// 단어가 영어인지 한글인지 판단한다.
		//-----------------------------------------------------
		while (ch=*strTemp++)
		{
			//-----------------------------------------------------
			// 한글인 경우
			//-----------------------------------------------------
			if (ch & 0x80)
			{
				if (*strTemp=='\0')
				{
					bKor = false;
					break;
				}

				strTemp++;

				bEng = false;
			}
			//-----------------------------------------------------
			// 영어인 경우
			//-----------------------------------------------------
			else if (ch>='a' && ch<='z')
			{
				bKor = false;
			}
			//-----------------------------------------------------
			// 아니면 .. 버린다.
			//-----------------------------------------------------
			else				
			{
				bEng = false;
				bKor = false;

				break;
			}
		}

		//-----------------------------------------------------
		// 영어..라고 판단된 경우
		//-----------------------------------------------------
		if (bEng)
		{
			m_mapCurseEng.Add(str);
		}
		//-----------------------------------------------------
		// 한글..이라고 판단된 경우
		//-----------------------------------------------------
		else if (bKor)
		{
			//-----------------------------------------------------
			// 한글 1,2,3,4자만 허용한다.
			//-----------------------------------------------------
			switch (strlen(str))
			{
				case 2 : m_mapCurseKor1.Add(str); break;
				case 4 : m_mapCurseKor2.Add(str); break;
				case 6 : m_mapCurseKor3.Add(str); break;
				case 8 : m_mapCurseKor4.Add(str); break;
			}
		}
	}

	file.close();
}

//----------------------------------------------------------------------
// Remove Curse
//----------------------------------------------------------------------
bool				
MChatManager::RemoveCurse(char* str, bool bForce) const
{
	#ifdef __GAME_CLIENT__
		if (!g_pUserOption->FilteringCurse && bForce == false || str==NULL)
		{
			return false;
		}
	#else
		if (str==NULL)
		{
			return false;
		}
	#endif


	bool existCurseEng = false;
	bool existCurseKor = false;

	int len = strlen(str);
	int i;
	int index;
	
	//const char* strMask = //"^^; -_-; !_!; o_O; *_*; m_m; u_u; p_q; =_=; -_+; $_$; v_v; Y_Y; o_o; O_O; w_w; #_#; ._.; n_n; &_&; @_@; 0_0; _-_; +_+; +_=; ~_~;";
	//	"#&*%!$#%&*@!%&$&#*&$@#*!%$&@#&*%!$#%&*@!%&$&#*&$@&#!%$*&%*#@#*!%$&@#&*%!$#%&*@!%&$&#*&$@#*!%$&@#&*%!$#%&*@!%&$&#*&$@#*!%$&@";

	//------------------------------------------------------------
	// 필요한 글자들만 걸러낸 string
	//------------------------------------------------------------
	char*	strFiltered = new char [len+1];		

	//------------------------------------------------------------
	// filter된 글자들의 원래 string에서의 index
	//------------------------------------------------------------
	int*	indexFiltered = new int [len+1];		

	//------------------------------------------------------------
	// 욕인지 아닌지 판단한다.
	//------------------------------------------------------------
	//bool*	isCurse = new bool [len+1];
	BYTE*	isCurse = new BYTE [len+1];
	

	//------------------------------------------------------------
	//
	//					영어욕 제거 
	//
	//------------------------------------------------------------
	// 알파벳만 남기고... lower char으로..
	// (!) string에서 욕을 찾는다.	
	//------------------------------------------------------------
	// hi, hello! f.u.c.k!~~!
	// --> hihellofuck
	//	 원본 string의 index를 가져야지 다른 문자로 치환하기가 쉽다.
	// 
	// 같은 욕이 여러번 있을수도 있으니까.. strstr을 여러번해야한다.	
	//------------------------------------------------------------
	char*	strFilteredPtr = strFiltered;
	int*	indexFilteredPtr = indexFiltered;
	BYTE*	isCursePtr = isCurse;
	
	char*	strOrg = str;	// 체크를 위해서..
	
	char	ch;
	const char toLower = 'a'-'A';

	//------------------------------------------------------------
	// 영어만 걸러내서 소문자로 바꾼다.
	//------------------------------------------------------------
	i = 0;
	index = 0;
	while (ch = *strOrg++)//, ch != '\0')
	{
		//----------------------------------------------
		// 소문자인 경우 --> 그대로 쓴다.
		//----------------------------------------------
		if (ch >= 'a' && ch <= 'z')
		{
			*strFilteredPtr++	= ch;
			*indexFilteredPtr++ = i;
			*isCursePtr++		= false;	// 초기화			
			index++;
		}		
		//----------------------------------------------
		// 대문자인 경우 --> 소문자로 바꾼다.
		//----------------------------------------------
		else if (ch >= 'A' && ch <= 'Z')
		{
			*strFilteredPtr++	= ch + toLower;
			*indexFilteredPtr++ = i;
			*isCursePtr++		= false;	// 초기화
			index++;
		}
		
		//----------------------------------------------
		// 다른 경우는 무시되는 문자이다.
		//----------------------------------------------

		i++;
	}
	*strFilteredPtr = '\0';
	
	//------------------------------------------------------------
	// 영어가 있는 경우만 영어욕을 제거한다.
	//------------------------------------------------------------
	if (index!=0)
	{
		MStringMap::const_iterator iString = m_mapCurseEng.begin();
		
		//------------------------------------------------------------
		// 모든 욕들에 대해서 다 비교한다... strFiltered에 있는지..
		//------------------------------------------------------------
		while (iString != m_mapCurseEng.end())
		{
			const MString* pString = iString->second;

			if (pString!=NULL)
			{
				strFilteredPtr = strFiltered;

				char* pFind = NULL;

				//---------------------------------------------------
				// strFiltered에서 욕을 찾는다.
				//---------------------------------------------------
				while (pFind = strstr( strFilteredPtr, pString->GetString() ))
				{					
					int lenCurse = pString->GetLength();

					//---------------------------------------------------
					// 찾았으면.. 표시해둔다.
					//---------------------------------------------------
					// 2004, 10, 26, sobeit modify start - 욕필터 수정
					//memset( isCurse+(pFind-strFilteredPtr), true, lenCurse);
					memset( isCurse+(pFind-strFilteredPtr), lenCurse, lenCurse);
					// 2004, 10, 26, sobeit modify end - 욕필터 수정
					
					//---------------------------------------------------
					// 다음에 검색할 위치를 지정한다.
					//---------------------------------------------------
					strFilteredPtr = pFind + lenCurse;

					existCurseEng = true;	// 욕 있다.
				}
			}

			iString++;
		}

		//------------------------------------------------------------
		// 찾아낸 욕들에 mask를 씌운다.
		//------------------------------------------------------------
		for (int i=0; i<index; i++)
		{
			if ( isCurse[i] )
			{
				// 욕이면.. 원래 string 위치에 mask를 씌운다.
				str[ indexFiltered[i] ] = s_MaskString[ i ];
			}
		}

	}

	
	//------------------------------------------------------------
	// 한글1자, 한글2자, 한글3자 ...  각각 따로..
	// 공백이나 특수문자 skip하면 띄워쓰기도 제거된다.
	// (1) 길이에 따라 나눠진 욕에서 string을 찾는다.
	//------------------------------------------------------------
	//     하이 뭐라고 우헤헤헤 안돼~~
	// --> 하이뭐라고우헤헤헤안돼
	//	 원본 string의 index를 가져야지 다른 문자로 치환하기가 쉽다.
	//
	//1글자( 하, 이, 뭐, 라, 고, 우, 헤, 헤, 헤, 안, 돼 )
	//2글자( 하이, 이뭐, 뭐라, 고우, 우헤, 헤헤, 헤헤, 헤안, 안돼 )
	//3글자(하이뭐, 이뭐라, 뭐라고, 라고우, 고우헤, 우헤헤, 헤헤헤, 헤헤안, 헤안돼)
	//4글자.... 등등
	//
	// 비교회수 : O( stringLength * (log(1글자욕수) + ... + log(n글자욕수)) )
	//
	// 예상) 40자 * (log(1000개)+log(1000개)+log(1000개)) = 40*30 = 120
	//
	//------------------------------------------------------------
	strFilteredPtr = strFiltered;
	indexFilteredPtr = indexFiltered;
	isCursePtr = isCurse;
	
	strOrg = str;	// 체크를 위해서..
	
	//------------------------------------------------------------
	// 한글만 걸러낸다.
	//------------------------------------------------------------
	i = 0;
	index = 0;
	while (ch = *strOrg++)//, ch != '\0')
	{
		//----------------------------------------------
		// 한글인 경우..
		//----------------------------------------------
		if (ch & 0x80)
		{
			char chNext = *strOrg++;

			//----------------------------------------------
			// 2 byte이므로.. 다음 byte로 체크한다.
			//----------------------------------------------
			if (chNext=='\0')
			{
				// 다음 한 byte가 없는 경우
				break;
			}
			
			//----------------------------------------------
			// 정상적인 한글 한자(2bytes)가 있는 경우
			//----------------------------------------------
			*strFilteredPtr++	= ch;
			*indexFilteredPtr++ = i++;
			*isCursePtr++		= false;	// 초기화

			*strFilteredPtr++	= chNext;
			*indexFilteredPtr++ = i;
			*isCursePtr++		= false;	// 초기화

			index+=2;
		}		
	
		//----------------------------------------------
		// 다른 경우는 무시되는 문자이다.
		//----------------------------------------------

		i++;
	}
	*strFilteredPtr = '\0';

	//------------------------------------------------------------
	// 글자수에 따라서.. 한글욕을 제거한다.
	//------------------------------------------------------------
	if (RemoveCurseKorean(strFiltered, 2, m_mapCurseKor1, isCurse))
	{
		existCurseKor = true;	// 욕 있다.
	}

	if (RemoveCurseKorean(strFiltered, 4, m_mapCurseKor2, isCurse))
	{
		existCurseKor = true;	// 욕 있다.
	}

	if (RemoveCurseKorean(strFiltered, 6, m_mapCurseKor3, isCurse))
	{
		existCurseKor = true;	// 욕 있다.
	}

	if (RemoveCurseKorean(strFiltered, 8, m_mapCurseKor4, isCurse))
	{
		existCurseKor = true;	// 욕 있다.
	}

	//------------------------------------------------------------
	// 한글 욕이 있다면..
	//------------------------------------------------------------
	if (existCurseKor)
	{
		//------------------------------------------------------------
		// 찾아낸 욕들에 mask를 씌운다.
		//------------------------------------------------------------
		
		for (int i=0; i<index; i++)
		{
			if ( isCurse[i] )
			{
				// 2004, 10, 26, sobeit modify start - 욕필터 수정
				
#ifdef __GAME_CLIENT__
				int j = 0;
				switch(isCurse[i])
				{
				case 2:
					{
						char* pChangeString = (*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_1].GetString();
						for(j = 0; j<(*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_1].GetLength() ; j++)
						{
							if( (i+j) < len && indexFiltered[i+j] < len && indexFiltered[i+j]>-1)
								str[ indexFiltered[i+j] ] = pChangeString[j];
						}
					}
					i+= 1;
					break;
				case 4:
					{
						char* pChangeString = (*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_2].GetString();
						for(j = 0; j<(*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_2].GetLength() ; j++)
						{
							if( (i+j) < len && indexFiltered[i+j] < len && indexFiltered[i+j]>-1)
								str[ indexFiltered[i+j] ] = pChangeString[j];
						}
					}
					i+= 3;
					break;
				case 6:
					{
						char* pChangeString = (*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_3].GetString();
						for(j = 0; j<(*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_3].GetLength() ; j++)
						{
							if( (i+j) < len && indexFiltered[i+j] < len && indexFiltered[i+j]>-1)
								str[ indexFiltered[i+j] ] = pChangeString[j];
						}
					}
					i+= 5;
					break;
				case 8:
					{
						char* pChangeString = (*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_4].GetString();
						for(j = 0; j<(*g_pGameStringTable)[UI_STRING_MESSAGE_REMOVE_CURSE_4].GetLength() ; j++)
						{
							if( (i+j) < len && indexFiltered[i+j] < len && indexFiltered[i+j]>-1)
								str[ indexFiltered[i+j] ] = pChangeString[j];
						}
					}
					i+= 7;
					break;
				default:
					str[ indexFiltered[i] ] = s_MaskString[ i ];
					break;
				}
#else
				// 욕이면.. 원래 string 위치에 mask를 씌운다.
				str[ indexFiltered[i] ] = s_MaskString[ i ];
#endif

				// 2004, 10, 26, sobeit modify end - 욕필터 수정
			}
		}
	}

	delete [] strFiltered;
	delete [] indexFiltered;
	delete [] isCurse;	

	//------------------------------------------------------------
	// 영어욕이나 한글욕이 있다면...
	//------------------------------------------------------------
	if (existCurseEng || existCurseKor)
	{
		return true;
	}

	return false;
}

//----------------------------------------------------------------------
// RemoveCurseKorean
//----------------------------------------------------------------------
// strKor	: 한글만 들어있는 string(공백,특수문자,영어,깨진한글.. 등은 없다!)
// lenCurse : 검색하려는 욕의 고정된 길이 byte수(한글이므로 2의 배수여야 한다)
// mapCurse : 욕들이 들어있는 MStringMap. 같은 길이의 욕들만 있다.
// isCurse	: 욕이 있는 위치의 정보
//----------------------------------------------------------------------
// --> 하이뭐라고우헤헤헤안돼
//1글자( 하, 이, 뭐, 라, 고, 우, 헤, 헤, 헤, 안, 돼 )
//2글자( 하이, 이뭐, 뭐라, 고우, 우헤, 헤헤, 헤헤, 헤안, 안돼 )
//3글자(하이뭐, 이뭐라, 뭐라고, 라고우, 고우헤, 우헤헤, 헤헤헤, 헤헤안, 헤안돼)
//----------------------------------------------------------------------
bool				
MChatManager::RemoveCurseKorean(const char* strKor, 
								int byteCurse, const MStringMap& mapCurse, 
								BYTE* isCurse) const
{
	int len = strlen(strKor);

	//---------------------------------------------------------
	// string 길이가 짧은 경우
	//---------------------------------------------------------
	if (len < byteCurse)
	{
		return false;
	}

	bool existCurse = false;

	//---------------------------------------------------------
	// 체크하면서 NULL을 찍기 때문에.. copy해서 사용한다.
	//---------------------------------------------------------
	char* strCheck = new char [len+1];
	strcpy( strCheck, strKor );
	
	char* strCheckPtr = strCheck;
	
	int maxCheck = len - (byteCurse-2);

	for (int i=0; i<maxCheck; i+=2)
	{
		//---------------------------------------------------------
		// 필요한 부분까지 체크하기 위해서 NULL을 찍는다.
		//---------------------------------------------------------
		char* strCheckNull = strCheckPtr + byteCurse;
		char previousNull = *strCheckNull;
		*strCheckNull = '\0';

		//---------------------------------------------------------
		// 선택한 단어가 욕map에 있는지 찾아본다.
		//---------------------------------------------------------
		MString tempStr(strCheckPtr);
		MStringMap::const_iterator iString = mapCurse.find( &tempStr );

		//---------------------------------------------------------
		// 욕인 경우 (정말 욕일까? - -;)
		//---------------------------------------------------------
		if (iString != mapCurse.end())
		{
			// 욕 길이만큼.. 욕이라고 체크해둔다.
			// 2004, 10, 26, sobeit modify start - 욕필터 수정
			//memset( isCurse+(strCheckPtr-strCheck), true, byteCurse);
			memset( isCurse+(strCheckPtr-strCheck), byteCurse, byteCurse);
			// 2004, 10, 26, sobeit modify end - 욕필터 수정

			existCurse = true;
		}		
	
		//---------------------------------------------------------
		// NULL로 해둔 부분을 원래대로 돌린다.
		//---------------------------------------------------------
		*strCheckNull = previousNull;

		strCheckPtr += 2;
	}

	delete [] strCheck;

	return existCurse;
}

//----------------------------------------------------------------------
// Add Mask
//----------------------------------------------------------------------
// str의 군데군데에 percent의 확률로 mask를 씌운다.
// 일단, s_MaskString2를 사용한다. - -;
// percent는 0~100 
// 0이면 다 마스크되고, 100이면 하나도 마스크되지 않는다.
//----------------------------------------------------------------------
void
MChatManager::AddMask(char* str, int percent) const
{
#ifdef OUTPUT_DEBUG
	percent = max( percent, 75 );
#endif

	if (percent >= 100)
	{
		return;
	}

	char ch;

	// 0 ~ 100 --> 0 ~ 63
	int pro = percent * 63 / 100;
	
	int index = rand() & 0x0F;

	while (ch = *str, ch != NULL)
	{
		int maskLen = 0;

		//-------------------------------------------------------
		// 공백인 경우는 무시한다.
		//-------------------------------------------------------
		if (ch==' ')
		{			
		}
		//-------------------------------------------------------
		// 한글인 경우
		//-------------------------------------------------------
		else if (ch & 0x80)
		{
			maskLen = 2;			
		}
		//-------------------------------------------------------
		// 한 문자.
		//-------------------------------------------------------
		else 
		{
			maskLen = 1;
		}		

		//-------------------------------------------------------
		// Mask할까?
		//-------------------------------------------------------
		if (maskLen!=0)
		{
			int bMask = (rand() & 0x3F) >= pro;	// 결정~

			if (bMask)
			{
				for (int i=0; i<maskLen; i++)
				{
					if (*str != NULL)
					{			
						*str = s_MaskString2[index++];
						str++;
					}					
				}
			}
			else
			{
				str += maskLen;
			}
		}
		else
		{
			str++;
		}
	}	
}
