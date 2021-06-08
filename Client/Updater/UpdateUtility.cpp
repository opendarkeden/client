//----------------------------------------------------------------------
// UpdateUtility.cpp
//----------------------------------------------------------------------
#include "Client_PCH.h"
#include <io.h>
#include <direct.h>
#include "UpdateUtility.h"
//#include "SPKFileLib.h"
#include "CFileIndexTable.h"


#define	NULL	0

//----------------------------------------------------------------------
// Pack의 Size의 byte 수
//----------------------------------------------------------------------
typedef	unsigned short		TYPE_PACKSIZE;
#define	SIZE_PACKSIZE		2

// 한번에 카피하는 byte수
#define	SIZE_BUFFER			4096


//----------------------------------------------------------------------
// Has Permission
//----------------------------------------------------------------------
// Directory나 File 접근시에.. 다른 Directory에 있는걸 건들이면 안된다.
//----------------------------------------------------------------------
bool
UUFHasPermission(const char* filename)
{	
	//------------------------------------------------------------
	// filename이 없을 경우 
	//------------------------------------------------------------
	if (filename==NULL)
	{	
		return false;
	}

	//------------------------------------------------------------
	// 첫 문자가 "\"이면 안된다. 
	// (최상위 디렉토리로 접근...가능한가? - -;; 그냥 함 해봄..)
	//------------------------------------------------------------
	if (filename[0]=='\\' || filename[0]=='/')
	{
		return false;
	}

	//------------------------------------------------------------
	// ":"이 들어가면 안된다. (드라이브를 바꿀 수 있다.)
	//------------------------------------------------------------
	if (strchr( filename, ':' )!=NULL)
	{
		return false;
	}
	
	//------------------------------------------------------------
	// ".." 이 들어가면 안된다. (상위 디렉토리이므로)
	//------------------------------------------------------------
//	if (strstr( filename, ".." )!=NULL)
//	{
//		return false;
//	}
   

	return true;
}

//----------------------------------------------------------------------
// Create Directory
//----------------------------------------------------------------------
// dirName : 현재 directory 밑에 생성할려는 directory 이름
//----------------------------------------------------------------------
// 새로운 Directory를 생성한다.
//----------------------------------------------------------------------
bool
UUFCreateDirectory(const char* dirName)
{
	// Directory 생성
	if (UUFHasPermission( dirName ))
	{
		// 제대로 생성한 경우
		if (_mkdir( dirName )==0)
		{
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------
// Remove Directory
//----------------------------------------------------------------------
// dirName : 지울려는 directory 이름
//----------------------------------------------------------------------
// Directory를 지운다.
//----------------------------------------------------------------------
bool
UUFDeleteDirectory(const char* dirName)
{
	if (UUFHasPermission( dirName ))
	{
		// 제대로 지운 경우
		if (_rmdir( dirName )==0)
		{
			return true;
		}
		// 뭔가 있어서 안 지워진 경우..
		else //if (errno!=ENOENT)
		{
			char CWD[_MAX_PATH];

			// 현재 directory를 기억해둔다.				
			GetCurrentDirectory(_MAX_PATH, CWD);
			
			if (_chdir( dirName ) == 0)
			{
				//---------------------------------------------------
				// file하나하나를 지워준다. T_T;
				//---------------------------------------------------
				struct _finddata_t	FileData;
				long				hFile;

				// 모든 화일을 읽어온다.
				if( (hFile = _findfirst( "*.*", &FileData )) != -1L )					
				{
					do
					{
						// .으로 시작하는 건 지울 필요 없당..
						if (FileData.name[0] != '.')
						{
							remove( FileData.name );
						}
					}
					while (_findnext( hFile, &FileData ) == 0);

					// 끝
					_findclose( hFile );			
				}
				else
				{
					return false;
				}
				
//				_chdir( "..\\" );
				SetCurrentDirectory(CWD);
				if (_rmdir( dirName )==0)
				{
					// 잘 지워졌다.
					return true;
				}
			}		
			else
			{
				return false;
			}
		}		
	}

	return false;
}

//----------------------------------------------------------------------
// Remove Files
//----------------------------------------------------------------------
// dirName : 지울려는 directory 이름
//----------------------------------------------------------------------
// Directory안의 파일들을 지운다.
//----------------------------------------------------------------------
bool
UUFDeleteFiles(const char *path, const char *fileext)
{
	char cwd[512];
	_getcwd(cwd, 512);

	if (_chdir( path ) == 0)
	{
		//---------------------------------------------------
		// file하나하나를 지워준다. T_T;
		//---------------------------------------------------
		struct _finddata_t	FileData;
		long				hFile;
		
		// 모든 화일을 읽어온다.
		if( (hFile = _findfirst( fileext, &FileData )) != -1L )					
		{
			do
			{
				// .으로 시작하는 건 지울 필요 없당..
				if (FileData.name[0] != '.')
				{
					remove( FileData.name );
				}
			}
			while (_findnext( hFile, &FileData ) == 0);
			
			// 끝
			_findclose( hFile );			
		}
		
	}		
	_chdir(cwd);

	return true;
}

//----------------------------------------------------------------------
// Copy File
//----------------------------------------------------------------------
// SourceFile : 원본
// TargetFile : 목적 filename
//----------------------------------------------------------------------
// SourceFile을 TargetFile로 copy해서 새로운 file을 생성한다.
//----------------------------------------------------------------------
bool
UUFCopyFile(const char* FilenameSource, const char* FilenameTarget)
{				
	if (UUFHasPermission( FilenameTarget ) && UUFHasPermission( FilenameSource))				
	{
		class ifstream fileSource(FilenameSource, ios::binary);
		class ofstream fileTarget(FilenameTarget, ios::binary);
		
		// 추가
		char buffer[SIZE_BUFFER];
		int n;
		
		//---------------------------------------------------------------
		// addFile을 읽어서 originalFile의 끝에 붙인다.
		//---------------------------------------------------------------
		while (1)
		{
			fileSource.read(buffer, SIZE_BUFFER);
			
			n = fileSource.gcount();

			if (n > 0)
			{		
				fileTarget.write(buffer, n);
			}
			else
			{
				break;
			}
		}

		fileSource.close();
		fileTarget.close();		

		return true;
	}

	return false;
}
					
//----------------------------------------------------------------------
// Move File
//----------------------------------------------------------------------
// SourceFile : 원본
// TargetFile : 목적 filename
//----------------------------------------------------------------------
// SourceFile을 TargetFile로 rename해서 새로운 file을 생성한다.
//----------------------------------------------------------------------
bool
UUFMoveFile(const char* FilenameSource, const char* FilenameTarget)
{				
	if (UUFHasPermission( FilenameTarget ) && UUFHasPermission( FilenameSource))				
	{
		ifstream file;

		file.open(FilenameTarget, ios::binary | ios::nocreate);
		
		if (file.is_open())
		{
			file.close();

			// 원래 있던게 있으면 지운다.
			if (remove( FilenameTarget )!=0)
			{
				// 못 지운 경우
				return false;
			}
		}		
	
		// 화일이름을 바꾼다.				
		if (rename( FilenameSource, FilenameTarget )==0)
		{
			return true;
		}		
	}

	return false;
}

//----------------------------------------------------------------------
// Delete File
//----------------------------------------------------------------------
// TargetFile : 삭제할 filename
//----------------------------------------------------------------------
// TargetFile을 지운다.
//----------------------------------------------------------------------
bool
UUFDeleteFile(const char* FilenameTarget)
{
	// file을 지운다.
	if (UUFHasPermission( FilenameTarget ))
	{
		if (remove( FilenameTarget )==0)
		{
			return true;
		}
	}

	return false;
}


//----------------------------------------------------------------------
// Rename File
//----------------------------------------------------------------------
// SourceFile : old filename
// TargetFile : new filename
//----------------------------------------------------------------------
// SourceFile을 TargetFile로 이름을 바꾼다.
// Directory name도 바꿀 수 있다.
//----------------------------------------------------------------------
bool
UUFRenameFile(const char* FilenameSource, const char* FilenameTarget)
{
	// 화일이름을 바꾼다.
	if (UUFHasPermission( FilenameTarget ))
	{
		if (rename( FilenameSource, FilenameTarget )==0)
		{
			return true;
		}
	}

	return false;
}


//----------------------------------------------------------------------
// Append Pack
//----------------------------------------------------------------------
// SourceFile		: Pack(to add)
// TargetFile		: Pack(original)
// SourceIndexFile	: PackIndex(to add)
// TargetIndexFile	: PackIndex(original)
//----------------------------------------------------------------------
// TargetFile의 끝에 SourceFile을 추가한다.
// 단, TargetFile의 전체 개수를 추가된 것 만큼 증가해야 한다.    
//----------------------------------------------------------------------
bool
UUFAppendPack(const char* FilenameAdd, const char* FilenameOriginal,
			const char* FilenameIndexAdd, const char* FilenameIndexOriginal)
{	
	//--------------------------------------------------
	// Index도 추가하는가?
	//--------------------------------------------------
	bool bAppendIndex;

	if (FilenameIndexAdd==NULL && FilenameIndexOriginal==NULL)
	{
		bAppendIndex = false;
	}
	else
	{
		bAppendIndex = true;
	}

	//--------------------------------------------------
	// permission 체크
	//--------------------------------------------------
	if (UUFHasPermission( FilenameOriginal ) 
		&& UUFHasPermission( FilenameAdd)

		// index가 없거나.. 있을때는 permission있어야 한다.
		&& (!bAppendIndex ||
			UUFHasPermission( FilenameIndexOriginal ) 
			&& UUFHasPermission( FilenameIndexAdd )
		))		
	{
		//---------------------------------------------------------------
		// 정해진 Filesize랑 같은가?
		//---------------------------------------------------------------
		//if (!IsFileSizeBeforeOK())
		//{
			// Update하려고 하는데 원하는 Filesize가 아니면.. 심각하다!!						
		//	return false;
		//}

		//---------------------------------------------------------------
		//
		//					Pack을 추가한다.
		//
		//---------------------------------------------------------------
		// 추가할 수 있게 한다.
		class ifstream addFile(FilenameAdd, ios::binary);
		class fstream originalFile(FilenameOriginal, ios::in | ios::out | ios::binary);

		TYPE_PACKSIZE	sourceCount, targetCount;
		
		//---------------------------------------------------------------
		// source의 개수를 저장해 둔다.
		//---------------------------------------------------------------
		addFile.read((char*)&sourceCount, SIZE_PACKSIZE);

		//---------------------------------------------------------------
		// target File Pointer를 끝으로..
		//---------------------------------------------------------------
		originalFile.seekp(0, ios::end);

		//---------------------------------------------------------------
		// Original File의 크기를 기억해둔다.
		//---------------------------------------------------------------
		long originalPackFileSize = originalFile.tellp();

		// 추가
		char buffer[SIZE_BUFFER];
		int n;
		//---------------------------------------------------------------
		// addFile을 읽어서 originalFile의 끝에 붙인다.
		//---------------------------------------------------------------
		while (1)
		{
			addFile.read(buffer, SIZE_BUFFER);
			
			n = addFile.gcount();

			if (n > 0)
			{		
				originalFile.write(buffer, n);
			}
			else
			{
				break;
			}
		}

		//---------------------------------------------------------------
		// 개수를 변경시켜준다. (originalFile + addFile)
		//---------------------------------------------------------------
		originalFile.seekg(0, ios::beg);
		originalFile.read((char*)&targetCount, SIZE_PACKSIZE);

		targetCount += sourceCount;

		originalFile.seekp(0, ios::beg);				
		originalFile.write((const char*)&targetCount, SIZE_PACKSIZE);

		// 끝
		addFile.close();
		originalFile.close();

		//---------------------------------------------------------------
		// 정해진 Filesize랑 같은가?
		//---------------------------------------------------------------
		//if (!IsFileSizeAfterOK())
		//{
			// Update를 했는데 원하는 Filesize가 아니면.. 심각하다!!
			// 심각한 경우이다.
		//	return false;
		//}

		
		//---------------------------------------------------------------
		//
		//					Pack Index를 추가한다.
		//
		//---------------------------------------------------------------	
		
		// Index를 추가할려는 경우에만..
		if (bAppendIndex)
		{
			//---------------------------------------------------------------
			// 정해진 Filesize랑 같은가?
			//---------------------------------------------------------------
			//if (!IsFileSizeBeforeOK())
			//{
				// Update하려고 하는데 원하는 Filesize가 아니면.. 심각하다!!						
			//	return false;
			//}


			// 추가할 수 있게 한다.
			class ifstream addIndexFile(FilenameIndexAdd, ios::binary);
			class fstream originalIndexFile(FilenameIndexOriginal, ios::in | ios::out | ios::binary);	

			TYPE_PACKSIZE	targetCount;

			//---------------------------------------------------------------
			// source index를 load한다.
			//---------------------------------------------------------------
			CFileIndexTable	sourceIndexFile;
			sourceIndexFile.LoadFromFile( addIndexFile );

			//---------------------------------------------------------------
			// target File Pointer를 끝으로..
			//---------------------------------------------------------------
			originalIndexFile.seekp(0, ios::end);
			
			//---------------------------------------------------------------
			// OriginalFile의 크기에서부터 file pointer가 시작되므로
			//---------------------------------------------------------------
			long targetEnd = originalPackFileSize - SIZE_PACKSIZE;

			//---------------------------------------------------------------
			// Source의 각 file position을 
			// targetEnd만큼 증가시켜서 originalIndexFile에 추가한다.
			//---------------------------------------------------------------
			long sourceIndex;
			for (int i=0; i<sourceIndexFile.GetSize(); i++)
			{
				sourceIndex = targetEnd + sourceIndexFile[i];
				originalIndexFile.write((const char*)&sourceIndex, 4);
			}								

			//---------------------------------------------------------------
			// 개수를 변경시켜준다. (originalIndexFile + addIndexFile)
			//---------------------------------------------------------------
			originalIndexFile.seekg(0, ios::beg);
			originalIndexFile.read((char*)&targetCount, SIZE_PACKSIZE);

			targetCount += sourceIndexFile.GetSize();

			originalIndexFile.seekp(0, ios::beg);				
			originalIndexFile.write((const char*)&targetCount, SIZE_PACKSIZE);

			// 끝
			addIndexFile.close();
			originalIndexFile.close();

			//---------------------------------------------------------------
			// 정해진 Filesize랑 같은가?
			//---------------------------------------------------------------
			//if (!IsFileSizeAfterOK())
			//{
				// Update를 했는데 원하는 Filesize가 아니면.. 심각하다!!
				// 심각한 경우이다.
			//	return false;
			//}	
		}

		return true;
	}

	return false;
}



//----------------------------------------------------------------------
// Append Info
//----------------------------------------------------------------------
// FilenameAdd		: 추가될 Information File
// FilenameOriginal : 원래의 Information File
//----------------------------------------------------------------------
// TargetFile의 끝에 SourceFile을 추가한다.
// 단, TargetFile의 전체 개수를 추가된 것 만큼 증가해야 한다.    
//----------------------------------------------------------------------
bool
UUFAppendInfo(const char* FilenameAdd, const char* FilenameOriginal)
{
	if (UUFHasPermission( FilenameOriginal ) && UUFHasPermission( FilenameAdd))				
	{			
		//------------------------------------------
		// 정해진 Filesize랑 같은가?
		//------------------------------------------
		//if (!IsFileSizeBeforeOK())
		//{
			// Update하려고 하는데 원하는 Filesize가 아니면.. 심각하다!!						
		//	return false;
		//}

		// 추가할 수 있게 한다.
		class ifstream sourceFile(FilenameAdd, ios::binary);
		class fstream targetFile(FilenameOriginal, ios::in | ios::out | ios::binary);
		

		int sourceCount, targetCount;
		// source의 개수를 저장해 둔다.
		sourceFile.read((char*)&sourceCount, 4);

		// target File Pointer를 끝으로..
		targetFile.seekp(0, ios::end);

		// 추가
		char buffer[SIZE_BUFFER];
		int n;
		// SourceFile을 읽어서 TargetFile의 끝에 붙인다.
		while (1)
		{
			sourceFile.read(buffer, SIZE_BUFFER);
			
			n = sourceFile.gcount();

			if (n > 0)
			{		
				targetFile.write(buffer, n);
			}
			else
			{
				break;
			}
		}

		// 개수를 변경시켜준다. (targetFile + sourceFile)
		targetFile.seekg(0, ios::beg);
		targetFile.read((char*)&targetCount, 4);

		targetCount += sourceCount;

		targetFile.seekp(0, ios::beg);				
		targetFile.write((const char*)&targetCount, 4);

		// 끝
		sourceFile.close();
		targetFile.close();


		//------------------------------------------
		// 정해진 Filesize랑 같은가?
		//------------------------------------------
		//if (!IsFileSizeAfterOK())
		//{
			// Update를 했는데 원하는 Filesize가 아니면.. 심각하다!!
			// 심각한 경우이다.
		//	return false;
		//}
	}

	return true;
}		

//----------------------------------------------------------------------
// Update SpritePack
//----------------------------------------------------------------------
// SpritePack에서 특정한 sprite들만 교체한다.
//----------------------------------------------------------------------
/*
bool		
UUFUpdateSpritePack(const char* FilenameNew, 
					const char* FilenameID, 
					const char* FilenameOriginal)
{
	if (UUFHasPermission( FilenameNew ) 
		&& UUFHasPermission( FilenameID )
		&& UUFHasPermission( FilenameOriginal ) )
	{
		int i;
		//-----------------------------------------------------
		// FilenameNew의 확장자를 찾는다.
		//-----------------------------------------------------
		int len = strlen(FilenameNew);
		char fileExtNew[10];
		fileExtNew[0] = '\0';
		for (i=len-1; i>=0; i--)
		{
			if (FilenameNew[i]=='.')
			{
				// 소문자로 저장
				char temp[80];
				strcpy(temp, FilenameNew+i+1);
				strcpy(fileExtNew, _strlwr( temp ));
				break;
			}
		}

		//-----------------------------------------------------
		// FilenameOriginal의 확장자를 찾는다.
		//-----------------------------------------------------
		len = strlen(FilenameOriginal);
		char fileExtOriginal[10];
		char filenameOriginal[80];
		fileExtOriginal[0] = '\0';
		for (i=len-1; i>=0; i--)
		{
			if (FilenameOriginal[i]=='.')
			{
				// 소문자로 저장
				char temp[80];
				strcpy(temp, FilenameOriginal+i+1);
				strcpy(fileExtOriginal, _strlwr( temp ));
				
				// filename을 구한다.
				for (int j=0; j<i; j++)
				{
					filenameOriginal[j] = FilenameOriginal[j];
				}
				filenameOriginal[i] = NULL;

				break;
			}
		}

		//-----------------------------------------------------
		// 두 화일의 확장자가 다른 경우는 
		// Update불가!
		//-----------------------------------------------------
		if (strcmp(fileExtNew, fileExtOriginal)!=0)
		{
			return false;
		}

		//-----------------------------------------------------
		//
		// id-set
		//
		//-----------------------------------------------------
		CSpriteIDTable	SIDT;
		class ifstream idFile(FilenameID, ios::binary);
		SIDT.LoadFromFile( idFile );
		idFile.close();

		//-----------------------------------------------------
		//
		//   SPK File Load
		//
		//-----------------------------------------------------
		CSpritePackBase* pSPKNew;
		CSpritePackBase* pSPKOriginal;
		
		//-----------------------------------------------------
		// SpritePack
		//-----------------------------------------------------
		if (strcmp(fileExtNew, "spk")==0)
		{
			pSPKNew = new CSpritePack;
			pSPKOriginal = new CSpritePack;
		}
		//-----------------------------------------------------
		// AlphaSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExtNew, "aspk")==0)
		{
			pSPKNew = new CAlphaSpritePack;
			pSPKOriginal = new CAlphaSpritePack;
		}
		//-----------------------------------------------------
		// IndexSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExtNew, "ispk")==0)
		{
			pSPKNew = new CIndexSpritePack;
			pSPKOriginal = new CIndexSpritePack;
		}
		//-----------------------------------------------------
		// ShadowSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExtNew, "sspk")==0)
		{
			pSPKNew = new CShadowSpritePack;
			pSPKOriginal = new CShadowSpritePack;
		}
		else
		{
			return false;
		}
		
		//-----------------------------------------------------
		//		
		// New SPK File
		//
		//-----------------------------------------------------
		class ifstream newSPKFile(FilenameNew, ios::binary);		
		pSPKNew->LoadFromFile( newSPKFile );
		newSPKFile.close();

		//-----------------------------------------------------
		//
		// Original SPK File
		//
		//-----------------------------------------------------		
		class ifstream originalSPKFile(FilenameOriginal, ios::binary);				
		pSPKOriginal->LoadFromFile( originalSPKFile );
		originalSPKFile.close();


		//-----------------------------------------------------
		//
		//   부분적으로 교체한다.
		//
		//-----------------------------------------------------
		for (i=0; i<SIDT.GetSize(); i++)
		{
			//-----------------------------------------------------
			// new에서 i번째는 
			// original에서 SIDT[i]번째 sprite가 된다.
			//-----------------------------------------------------
			*((*pSPKOriginal)[ SIDT[i] ]) = *((*pSPKNew)[i]);
		}

		//-----------------------------------------------------
		// Save : 바뀐 것을 다시 저장한다.
		//-----------------------------------------------------
		char indexFilename[80];
		sprintf(indexFilename, "%s.i%s", filenameOriginal, fileExtNew);

		class ofstream originalSPKFile2(FilenameOriginal, ios::binary);
		class ofstream originalSPKIndexFile2(indexFilename, ios::binary);
	
		pSPKOriginal->SaveToFile( originalSPKFile2, originalSPKIndexFile2 );

		originalSPKFile2.close();
		originalSPKIndexFile2.close();

		//-----------------------------------------------------
		// 메모리에서 제거
		//-----------------------------------------------------
		delete pSPKNew;
		delete pSPKOriginal;		
	}

	return true;
}

//----------------------------------------------------------------------
// Delete SpritePack
//----------------------------------------------------------------------
// SpritePack에서 특정한 Sprite들을 삭제한다.
//----------------------------------------------------------------------
bool		
UUFDeleteSpritePack(const char* FilenameID, const char* FilenameOriginal)
{
	if (UUFHasPermission( FilenameID )
		&& UUFHasPermission( FilenameOriginal ) )
	{
		//-----------------------------------------------------
		// FilenameOriginal의 확장자를 찾는다.
		//-----------------------------------------------------
		int len = strlen(FilenameOriginal);
		char fileExtOriginal[10];
		char filenameOriginal[80];
		fileExtOriginal[0] = '\0';
		for (int i=len-1; i>=0; i--)
		{
			if (FilenameOriginal[i]=='.')
			{
				// 소문자로 저장
				char temp[80];
				strcpy(temp, FilenameOriginal+i+1);
				strcpy(fileExtOriginal, _strlwr( temp ));
				
				// filename을 구한다.
				for (int j=0; j<i; j++)
				{
					filenameOriginal[j] = FilenameOriginal[j];
				}
				filenameOriginal[i] = NULL;

				break;
			}
		}

		//-----------------------------------------------------
		//
		//   SPK File Load
		//
		//-----------------------------------------------------
		CSpritePackBase* pSPKOriginal;
		
		//-----------------------------------------------------
		// SpritePack
		//-----------------------------------------------------
		if (strcmp(fileExtOriginal, "spk")==0)
		{
			pSPKOriginal = new CSpritePack;
		}
		//-----------------------------------------------------
		// AlphaSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExtOriginal, "aspk")==0)
		{
			pSPKOriginal = new CAlphaSpritePack;
		}
		//-----------------------------------------------------
		// IndexSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExtOriginal, "ispk")==0)
		{
			pSPKOriginal = new CIndexSpritePack;
		}
		//-----------------------------------------------------
		// ShadowSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExtOriginal, "sspk")==0)
		{
			pSPKOriginal = new CShadowSpritePack;
		}
		else
		{
			return false;
		}
		

		//-----------------------------------------------------
		//
		// Original SPK File
		//
		//-----------------------------------------------------		
		class ifstream originalSPKFile(FilenameOriginal, ios::binary);				
		pSPKOriginal->LoadFromFile( originalSPKFile );
		originalSPKFile.close();

		
		//-----------------------------------------------------
		//
		// id-set
		//
		//-----------------------------------------------------
		CSpriteIDTable	SIDT;
		class ifstream idFile(FilenameID, ios::binary);
		SIDT.LoadFromFile( idFile );
		idFile.close();

		//-----------------------------------------------------
		//
		//   제거한다. == 다시 Save할때 뺀다..
		//
		//-----------------------------------------------------
		//-----------------------------------------------------
		// Save : 바뀐 것을 다시 저장한다.
		//-----------------------------------------------------
		char indexFilename[80];
		sprintf(indexFilename, "%s.i%s", filenameOriginal, fileExtOriginal);

		class ofstream spkFile(FilenameOriginal, ios::binary);
		class ofstream indexFile(indexFilename, ios::binary);
	
		//--------------------------------------------------
		// index file을 생성하기 위한 정보
		//--------------------------------------------------
		long*	pIndex = new long [pSPKOriginal->GetSize()];

		//--------------------------------------------------
		// Size 저장
		//--------------------------------------------------
		TYPE_SPRITEID size = pSPKOriginal->GetSize() - SIDT.GetSize();

		spkFile.write((const char *)&size, SIZE_SPRITEID); 
		indexFile.write((const char *)&size, SIZE_SPRITEID); 

		//--------------------------------------------------
		// SpritePack에서 SIDT에 없는 것만 저장한다.
		//--------------------------------------------------
		int k=0;
		for (i=0; i<pSPKOriginal->GetSize(); i++)
		{
			//--------------------------------------------------
			// 있는지 없는지 본다..
			//--------------------------------------------------
			bool bDelete = false;
			// 음.. 이런 무식한 알고리즘을 쓰다니..
			// 아. 귀찮다.. - -;;
			for (int j=0; j<SIDT.GetSize(); j++)
			{
				if (i == SIDT[j])
				{
					bDelete = true;
					break;
				}
			}

			//--------------------------------------------------
			// 지워야 하는 것이면 skip한다.
			//--------------------------------------------------
			if (bDelete)
			{
				continue;
			}

			//--------------------------------------------------
			// SpritePack file에 쓰여지는 index를 저장
			//--------------------------------------------------
			pIndex[k++] = spkFile.tellp();

			//--------------------------------------------------
			// Sprite저장
			//--------------------------------------------------
			((*pSPKOriginal)[i])->SaveToFile(spkFile);		// CSprite저장				
		}

		if (k!=size)
		{
			// 심각한 경우다. - -;;
			return false;
		}

		//--------------------------------------------------
		// index 저장
		//--------------------------------------------------
		for (i=0; i<size; i++)
		{
			indexFile.write((const char*)&pIndex[i], 4);
		}

		delete [] pIndex;


		spkFile.close();
		indexFile.close();

		//-----------------------------------------------------
		// 메모리에서 제거
		//-----------------------------------------------------
		delete pSPKOriginal;		
	}

	return true;
}

//----------------------------------------------------------------------
// Make SpritePack Index
//----------------------------------------------------------------------
// SpritePack의 Index를 생성한다.
//----------------------------------------------------------------------
bool		
UUFMakeSpritePackIndex(const char* Filename)
{
	if (UUFHasPermission( Filename ) )
	{
		int i;
		//-----------------------------------------------------
		// Filename의 확장자를 찾는다.
		//-----------------------------------------------------
		int len = strlen(Filename);
		char filename[80];
		char fileExt[10];
		fileExt[0] = '\0';
		for (i=len-1; i>=0; i--)
		{
			if (Filename[i]=='.')
			{
				// 소문자로 저장
				char temp[80];
				strcpy(temp, Filename+i+1);
				strcpy(fileExt, _strlwr( temp ));

				// filename을 구한다.
				for (int j=0; j<i; j++)
				{
					filename[j] = Filename[j];
				}
				filename[i] = NULL;

				break;
			}
		}

		//-----------------------------------------------------
		//
		//   SPK File Load
		//
		//-----------------------------------------------------
		CSpritePackBase* pSPK;
		
		//-----------------------------------------------------
		// SpritePack
		//-----------------------------------------------------
		if (strcmp(fileExt, "spk")==0)
		{
			pSPK = new CSpritePack;			
		}
		//-----------------------------------------------------
		// AlphaSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExt, "aspk")==0)
		{
			pSPK = new CAlphaSpritePack;			
		}
		//-----------------------------------------------------
		// IndexSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExt, "ispk")==0)
		{
			pSPK = new CIndexSpritePack;			
		}
		//-----------------------------------------------------
		// ShadowSpritePack
		//-----------------------------------------------------
		else if (strcmp(fileExt, "sspk")==0)
		{
			pSPK = new CShadowSpritePack;			
		}
		else
		{
			return false;
		}
		
		//-----------------------------------------------------
		//		
		//  SPK File
		//
		//-----------------------------------------------------
		class ifstream SPKFile(Filename, ios::binary);		
		pSPK->LoadFromFile( SPKFile );
		SPKFile.close();

		//-----------------------------------------------------
		// Save : 다시 저장한다. --> Index생성
		//-----------------------------------------------------
		char indexFilename[80];
		sprintf(indexFilename, "%s.%si", filename, fileExt);

		class ofstream SPKFile2(Filename, ios::binary);
		class ofstream SPKIndexFile2(indexFilename, ios::binary);
	
		pSPK->SaveToFile( SPKFile2, SPKIndexFile2 );

		SPKFile2.close();
		SPKIndexFile2.close();

		//-----------------------------------------------------
		// 메모리에서 제거
		//-----------------------------------------------------
		delete pSPK;
	}

	return true;
}
*/