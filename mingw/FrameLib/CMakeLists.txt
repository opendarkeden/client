add_library(FrameLib CAnimationFrame.cpp  CAnimationFrameSetManager.cpp  CCreatureFrameSetManager.cpp  CFrame.cpp  CFramePack.cpp  CFrameSetManager.cpp  Frame_PCH.cpp)

target_include_directories(FrameLib PUBLIC ../SpriteLib ..)
