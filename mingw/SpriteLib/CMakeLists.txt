add_library(SpriteLib CAlphaSprite.cpp      CAlphaSpritePackList.cpp     CFileIndexTable.cpp  CIndexSprite555.cpp CSpriteDef.cpp             CSpriteSet.cpp         MPalette.cpp
CAlphaSprite555.cpp   CFilter.cpp          CIndexSprite565.cpp   CSprite.cpp CSpriteOutlineManager.cpp  CSpriteSetManager.cpp  MPalettePack.cpp
CAlphaSprite565.cpp    CIndexSpritePack.cpp  CSprite555.cpp         CSpritePack.cpp            CSpritePal.cpp          CSpriteSurface.cpp
CAlphaSpritePal.cpp          CIndexSprite.cpp     CShadowSprite.cpp     CSprite565.cpp         CSpritePackList.cpp        CSpritePalBase.cpp      CStorageSurface.cpp)

target_include_directories(SpriteLib PUBLIC ../DXLib ..)
