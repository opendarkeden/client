//----------------------------------------------------------------------
// UpdateUtility.h
//----------------------------------------------------------------------

#ifndef	__UPDATEUTILITY_H__
#define	__UPDATEUTILITY_H__

//----------------------------------------------------------------------
// Update Utility Functions
//----------------------------------------------------------------------
//----------------------------------------------------------------------
// Permission 체크 - 다른 directory에는 접근을 못하도록
//----------------------------------------------------------------------
bool		UUFHasPermission(const char* filename);


//----------------------------------------------------------------------
// Directory 생성/삭제
//----------------------------------------------------------------------
bool		UUFCreateDirectory(const char* dirName);
bool		UUFDeleteDirectory(const char* dirName);


//----------------------------------------------------------------------
// File		Copy / Move / Delete / Rename(Directory도 가능)
//----------------------------------------------------------------------
bool		UUFCopyFile(const char* FilenameSource, const char* FilenameTarget);
bool		UUFMoveFile(const char* FilenameSource, const char* FilenameTarget);
bool		UUFDeleteFile(const char* FilenameTarget);
bool		UUFDeleteFiles(const char *path, const char *fileext);
bool		UUFRenameFile(const char* FilenameSource, const char* FilenameTarget);


//----------------------------------------------------------------------
// Append Pack (with Index)
//----------------------------------------------------------------------
// IndexFile들이 NULL이면 Index는 추가하지 않는다.
//----------------------------------------------------------------------
bool		UUFAppendPack(const char* FilenameAdd, const char* FilenameOriginal,
						const char* FilenameIndexAdd=0, const char* FilenameIndexOriginal=0);

//----------------------------------------------------------------------
// Delete SpritePack
//----------------------------------------------------------------------
// SpritePack에서 특정한 Sprite들을 삭제한다.
//----------------------------------------------------------------------
//bool		UUFDeleteSpritePack(const char* FilenameID, const char* FilenameOriginal);

//----------------------------------------------------------------------
// Update SpritePack
//----------------------------------------------------------------------
// SpritePack에서 특정한 sprite들만 교체한다.
//----------------------------------------------------------------------
//bool		UUFUpdateSpritePack(const char* FilenameNew, 
//								const char* FilenameID, 
//								const char* FilenameOriginal);

//----------------------------------------------------------------------
// Append Info
//----------------------------------------------------------------------
bool		UUFAppendInfo(const char* FilenameAdd, const char* FilenameOriginal);

//----------------------------------------------------------------------
// Make SpritePack Index
//----------------------------------------------------------------------
// SpritePack을 load해서 index를 생성한다.
//----------------------------------------------------------------------
//bool		UUFMakeSpritePackIndex(const char* Filename);

#endif