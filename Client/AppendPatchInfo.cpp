//-----------------------------------------------------------------------------
// AppendPatchInfo.cpp
//-----------------------------------------------------------------------------
#include "Client_PCH.h"
#include "AppendPatchInfo.h"

#ifdef __GAME_CLIENT__
	extern void		UpdateProgressBar();
	extern void		SetProgressBarText(const char* title);
#endif

//-----------------------------------------------------------------------------
//s
//						APPEND_PATCH_NODE
//
//-----------------------------------------------------------------------------
bool		
APPEND_PATCH_NODE::Append(const char* orgFilename) const
{	
	//-------------------------------------------------------------
	// append화일 체크..
	//-------------------------------------------------------------
	std::ifstream appendFile(appendFilename.GetString(), std::ios::binary );		

	int n;
	char buffer[4096];

	// 우훗.. - -;;
	#ifdef __GAME_CLIENT__
		UpdateProgressBar();
	#endif

	//-------------------------------------------------------------
	// 패치 화일이 있으면 걍 패치..
	//-------------------------------------------------------------
	if (appendFile.is_open())
	{
		#ifdef __GAME_CLIENT__
			SetProgressBarText("패치 화일을 적용시키고 있습니다.");
		#endif

		std::fstream orgFile(orgFilename, std::ios::binary | std::ios::ate | std::ios::in | std::ios::out);
		
		if (orgFile.is_open())
		{
			WORD orgSpkSize;
			orgFile.seekg( 0 );
			orgFile.read((char*)&orgSpkSize, 2);	// Sprite의 개수

			appendFile.seekg( 2 );	// size부분 제외
			orgFile.seekp( writePosition );		
			
			while (1)
			{
				appendFile.read(buffer, 4096);
				n = appendFile.gcount();

				if (n > 0)
				{
					orgFile.write(buffer, n);
				}
				else
				{
					break;
				}
			}		

			appendFile.close();
			
			// 개수 변경
			orgFile.seekp( 0, std::ios::beg );
			WORD afterNum = max(afterSpkSize, orgSpkSize);	// 큰 개수로 설정 - -;

			orgFile.write((const char*)&afterSpkSize, 2);
			orgFile.close();

			// 패치화일을 지운다.
			remove( appendFilename.GetString() );
		}
		else
		{
			return false;
		}
	}	

	return true;
}

//-----------------------------------------------------------------------------
// Save To File
//-----------------------------------------------------------------------------
void		
APPEND_PATCH_NODE::SaveToFile(std::ofstream& file)
{
	appendFilename.SaveToFile( file );
	file.write((const char*)&appendFilesize, 4);
	file.write((const char*)&appSpkSize, 2);
	file.write((const char*)&writePosition, 4);
	file.write((const char*)&afterFilesize, 4);
	file.write((const char*)&afterSpkSize, 2);
}

//-----------------------------------------------------------------------------
// Load From File
//-----------------------------------------------------------------------------
void		
APPEND_PATCH_NODE::LoadFromFile(std::ifstream& file)
{
	appendFilename.LoadFromFile( file );
	file.read((char*)&appendFilesize, 4);
	file.read((char*)&appSpkSize, 2);
	file.read((char*)&writePosition, 4);
	file.read((char*)&afterFilesize, 4);
	file.read((char*)&afterSpkSize, 2);
}


//-----------------------------------------------------------------------------
//
//							AppendPatch
//
//-----------------------------------------------------------------------------
AppendPatch::AppendPatch()
{
	m_orgFilesize = 0;
	m_orgSpkSize = 0;

	m_finalFilesize = 0;
	m_finalSpkSize = 0;
}

AppendPatch::~AppendPatch()
{
	Release();
}

//-----------------------------------------------------------------------------
// Release
//-----------------------------------------------------------------------------
void
AppendPatch::Release()
{
	APPEND_PATCH_VECTOR::iterator iNode = m_AppendPatch.begin();

	while (iNode != m_AppendPatch.end())
	{
		delete *iNode;

		iNode ++;
	}

	m_AppendPatch.clear();

	m_orgFilename.Release();
	m_orgFilesize = 0;
	m_orgSpkSize = 0;
	m_finalFilesize = 0;
	m_finalSpkSize = 0;
}

//-----------------------------------------------------------------------------
// Set OriginalInfo
//-----------------------------------------------------------------------------
void		
AppendPatch::SetOriginalInfo(const char* orgFilename, long orgFilesize, WORD orgSpkSize)
{
	m_orgFilename = orgFilename;

	m_orgFilesize = orgFilesize;
	m_orgSpkSize = orgSpkSize;
}

//-----------------------------------------------------------------------------
// Set FinalInfo
//-----------------------------------------------------------------------------
void		
AppendPatch::SetFinalInfo(long finalFilesize, WORD finalSpkSize)
{
	m_finalFilesize = finalFilesize;
	m_finalSpkSize = finalSpkSize;
}

//-----------------------------------------------------------------------------
// Set FinalInfo
//-----------------------------------------------------------------------------
// currentFilename에서 정보를 읽어들인다.
//-----------------------------------------------------------------------------
bool		
AppendPatch::SetFinalInfo(const char* currentFilename)
{
	std::ifstream file(currentFilename, std::ios::binary );

	if (!file.is_open())
	{
		return false;
	}

	// appSpkSize - Pack 개수
	file.read((char*)&m_finalSpkSize, 2);
	file.seekg( 0, std::ios::end );

	// appendFilesize - AppendPack 크기
	m_finalFilesize = file.tellg();	
	
	file.close();

	return true;
}

//-----------------------------------------------------------------------------
// Add AppendInfo
//-----------------------------------------------------------------------------
void		
AppendPatch::AddAppendInfo(const char* appendFilename, long appendFilesize, WORD appSpkSize)
{
	long previousFilesize;
	WORD previousSpkSize;

	//------------------------------------------------------------
	// 바로 전 상태의 정보
	//------------------------------------------------------------
	if (m_AppendPatch.size()==0)
	{
		previousFilesize = m_orgFilesize;
		previousSpkSize	= m_orgSpkSize;
	}
	else
	{
		APPEND_PATCH_NODE* pNode = m_AppendPatch.back();

		previousFilesize = pNode->afterFilesize;
		previousSpkSize = pNode->afterSpkSize;
	}

	//------------------------------------------------------------
	// 현재 정보 설정
	//------------------------------------------------------------
	APPEND_PATCH_NODE* pNode = new APPEND_PATCH_NODE;

	pNode->appendFilename	= appendFilename;
	pNode->appendFilesize	= appendFilesize;
	pNode->appSpkSize		= appSpkSize;		
	pNode->writePosition	= previousFilesize;
	pNode->afterFilesize	= previousFilesize + appendFilesize-2;	// SPK의 개수 부분 2 bytes
	pNode->afterSpkSize		= previousSpkSize + appSpkSize;

	m_AppendPatch.push_back( pNode );
}

//-----------------------------------------------------------------------------
// Add AppendInfo
//-----------------------------------------------------------------------------
// currentFilename은 현재 화일의 위치이다.
// appendPatch를 할 위치는 m_orgFilename을 참고해야 한다.
//-----------------------------------------------------------------------------
bool
AppendPatch::AddAppendInfo(const char* currentFilename)
{
	//------------------------------------------------------------
	// 필요한 정보
	//------------------------------------------------------------
	char appendFilename[256];
	long appendFilesize;
	WORD appSpkSize;

	std::ifstream file(currentFilename, std::ios::binary );

	if (!file.is_open())
	{
		return false;
	}

	// appSpkSize - Pack 개수
	file.read((char*)&appSpkSize, 2);
	file.seekg( 0, std::ios::end );

	// appendFilesize - AppendPack 크기
	appendFilesize = file.tellg();
	
	file.close();

	// appendFilename
	strcpy(appendFilename, m_orgFilename.GetString());
	char* pBS = strrchr(appendFilename, '\\');
	if (pBS==NULL)
	{
		// ? -_-;;
		return false;
	}
	else
	{
		*(pBS+1) = '\0';		// 'Data\\Image\\' 까지
	}

	pBS = (char*)strrchr(currentFilename, '\\');	// '\\NewXX.spk'
	if (pBS==NULL)
	{
		strcat(appendFilename, currentFilename);
	}
	else
	{
		strcat(appendFilename, pBS+1);		// NewXX.spk
	}

	long previousFilesize;
	WORD previousSpkSize;

	//------------------------------------------------------------
	// 바로 전 상태의 정보
	//------------------------------------------------------------
	if (m_AppendPatch.size()==0)
	{
		previousFilesize = m_orgFilesize;
		previousSpkSize	= m_orgSpkSize;
	}
	else
	{
		APPEND_PATCH_NODE* pNode = m_AppendPatch.back();

		previousFilesize = pNode->afterFilesize;
		previousSpkSize = pNode->afterSpkSize;
	}

	//------------------------------------------------------------
	// 현재 정보 설정
	//------------------------------------------------------------
	APPEND_PATCH_NODE* pNode = new APPEND_PATCH_NODE;

	pNode->appendFilename	= appendFilename;
	pNode->appendFilesize	= appendFilesize;
	pNode->appSpkSize		= appSpkSize;		
	pNode->writePosition	= previousFilesize;
	pNode->afterFilesize	= previousFilesize + appendFilesize-2;	// SPK의 개수 부분 2 bytes
	pNode->afterSpkSize		= previousSpkSize + appSpkSize;

	m_AppendPatch.push_back( pNode );

	return true;
}

//-----------------------------------------------------------------------------
// Execute Patch
//-----------------------------------------------------------------------------
bool
AppendPatch::ExecutePatch() const
{
	APPEND_PATCH_VECTOR::const_iterator iNode = m_AppendPatch.begin();

	while (iNode != m_AppendPatch.end())
	{
		APPEND_PATCH_NODE* pNode = *iNode;

		if (!pNode->Append(m_orgFilename.GetString()))
		{
			return false;
		}
	
		iNode++;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Check FinalInfo
//-----------------------------------------------------------------------------
bool
AppendPatch::CheckFinalInfo() const
{
	std::ifstream orgFile(m_orgFilename.GetString(), std::ios::binary );
		
	if (orgFile.is_open())
	{
		WORD spkSize;
		orgFile.read((char*)&spkSize, 2);	// Sprite의 개수

		if (spkSize != m_finalSpkSize)
		{
			return false;
		}

		orgFile.seekg( 0, std::ios::end );
		long fpEnd = orgFile.tellg();

		if (fpEnd != m_finalFilesize)
		{
			return false;
		}
		
		orgFile.close();
	}

	return true;
}

//-----------------------------------------------------------------------------
// Calculate FinalInfo 
//-----------------------------------------------------------------------------
// 계산상으로 맞는지 체크한다.
//-----------------------------------------------------------------------------
bool
AppendPatch::CalculateFinalInfo() const
{
	APPEND_PATCH_VECTOR::const_iterator iNode = m_AppendPatch.begin();

	long totalFilesize	= m_orgFilesize;
	WORD totalSpkSize	= m_orgSpkSize;

	while (iNode != m_AppendPatch.end())
	{
		APPEND_PATCH_NODE* pNode = *iNode;

		totalFilesize += pNode->appendFilesize - 2;	// -2 for spkNum
		totalSpkSize += pNode->appSpkSize;
		
		iNode++;
	}

	if (m_finalFilesize==totalFilesize
		&& m_finalSpkSize==totalSpkSize)
	{
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------
// Save To File
//-----------------------------------------------------------------------------
void		
AppendPatch::SaveToFile(std::ofstream& file)
{	
	m_orgFilename.SaveToFile( file );
	file.write((const char*)&m_orgFilesize, 4);
	file.write((const char*)&m_orgSpkSize, 2);
	file.write((const char*)&m_finalFilesize, 4);
	file.write((const char*)&m_finalSpkSize, 2);

	int num = m_AppendPatch.size();
	file.write((const char*)&num, 4);

	APPEND_PATCH_VECTOR::const_iterator iNode = m_AppendPatch.begin();

	while (iNode != m_AppendPatch.end())
	{
		APPEND_PATCH_NODE* pNode = *iNode;

		pNode->SaveToFile( file );
		
		iNode++;
	}	
}

//-----------------------------------------------------------------------------
// Load From File
//-----------------------------------------------------------------------------
void		
AppendPatch::LoadFromFile(std::ifstream& file)
{
	Release();

	m_orgFilename.LoadFromFile( file );
	file.read((char*)&m_orgFilesize, 4);
	file.read((char*)&m_orgSpkSize, 2);
	file.read((char*)&m_finalFilesize, 4);
	file.read((char*)&m_finalSpkSize, 2);

	int num;
	file.read((char*)&num, 4);

	for (int i=0; i<num; i++)
	{
		APPEND_PATCH_NODE* pNode = new APPEND_PATCH_NODE;

		pNode->LoadFromFile( file );
		
		m_AppendPatch.push_back( pNode );
	}
}


//-----------------------------------------------------------------------------
// GetAllSize
//-----------------------------------------------------------------------------
int
AppendPatchTable::GetAllSize() const
{
	int allSize = 0;

	for (int i=0; i<m_Size; i++)
	{
		allSize += m_pTypeInfo[i].GetSize();
	}

	return allSize;
}

