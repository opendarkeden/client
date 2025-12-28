/* ACProtect*/
#define GetRegistrationName(szRegistrationName)  ::MessageBox (HWND(-1),szRegistrationName,NULL,0);
#define GetTrialUsageTimes(lpUsageTime)          ::MessageBox (HWND(-1),lpUsageTime       ,NULL,1);

#define EMBEDDED_BEGIN \
  __asm pushad\
  __asm __emit 0x6a \
  __asm __emit 0x05 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0xff \
  __asm call dword ptr [RegisterHotKey]\
  __asm     popad \
  __asm     pushad \
  __asm __emit 0xe9 \
  __asm __emit 0x1c \
  __asm __emit 0x32 \
  __asm __emit 0 \
  __asm __emit 0 \
  __asm __emit 'p'\
  __asm __emit 'e'\
  __asm __emit 'e'\
  __asm __emit 't'\
  __asm __emit 'l'\
  __asm __emit 'e'\
  __asm __emit 's'\
  __asm __emit '_'\
  __asm __emit '_'\
  __asm __emit 'h'\
  __asm __emit 'e'\
  __asm __emit 'a'\
  __asm __emit 'd'\
  __asm __emit 'e'\
  __asm __emit 'r'\
  embedded_hole\
  __asm __emit 'p'\
  __asm __emit 'e'\
  __asm __emit 'e'\
  __asm __emit 't'\
  __asm __emit 'l'\
  __asm __emit 'e'\
  __asm __emit 's'\
  __asm __emit '_'\
  __asm __emit 'b'\
  __asm __emit 'e'\
  __asm __emit 'g'\
  __asm __emit 'i'\
  __asm __emit 'n'\
  __asm __emit 0x61

 #define EMBEDDED_END \
  __asm pushad\
  __asm __emit 0x6a \
  __asm __emit 0x04 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0xff \
  __asm call dword ptr [RegisterHotKey]\
  __asm __emit 235 \
  __asm __emit 30 \
  __asm __emit 'p'\
  __asm __emit 'e'\
  __asm __emit 'e'\
  __asm __emit 't'\
  __asm __emit 'l'\
  __asm __emit 'e'\
  __asm __emit 's'\
  __asm __emit '_'\
  __asm __emit 'e'\
  __asm __emit 'n'\
  __asm __emit 'd'\
  fourzero\
  fourzero\
  fourzero\
  fourzero\
  __asm add byte ptr [eax],al\
  __asm __emit 0\
  __asm __emit 0x61

 #define RSALOCK_BEGIN \
  __asm pushad \
  __asm __emit 0x6a \
  __asm __emit 0x02 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0xff \
  __asm call dword ptr [RegisterHotKey]\
  __asm __emit 0xeb \
  __asm __emit 0x0d \
  __asm __emit 'R'\
  __asm __emit 'e'\
  __asm __emit 'g'\
  __asm __emit 'O'\
  __asm __emit 'n'\
  __asm __emit 'l'\
  __asm __emit 'y'\
  __asm __emit '_'\
  __asm __emit 'b'\
  __asm __emit 'e'\
  __asm __emit 'g'\
  __asm __emit 'i'\
  __asm __emit 'n'\
  __asm __emit 0x61

  #define RSALOCK_END\
  __asm __emit 0x60 \
  __asm __emit 0xeb \
  __asm __emit 0x0b \
  __asm __emit 'R'\
  __asm __emit 'e'\
  __asm __emit 'g'\
  __asm __emit 'O'\
  __asm __emit 'n'\
  __asm __emit 'l'\
  __asm __emit 'y'\
  __asm __emit '_'\
  __asm __emit 'e'\
  __asm __emit 'n'\
  __asm __emit 'd'\
  __asm __emit 0x6a \
  __asm __emit 0x03 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0x00 \
  __asm __emit 0x6a \
  __asm __emit 0xff \
  __asm call dword ptr [RegisterHotKey]\
  __asm __emit 0x61

   #define DYNAMIC_BEGIN \
  __asm __emit 0x60 \
  __asm __emit 0xe9 \
  __asm __emit 0xaa \
  __asm __emit 0x01 \
  __asm __emit 0 \
  __asm __emit 0 \
  __asm __emit 'd'\
  __asm __emit 'y'\
  __asm __emit 'm'\
  __asm __emit 'l'\
  __asm __emit 'c'\
  __asm __emit 'k'\
  __asm __emit '_'\
  __asm __emit '_'\
  __asm __emit 'h'\
  __asm __emit 'e'\
  __asm __emit 'a'\
  __asm __emit 'd'\
  __asm __emit 'e'\
  __asm __emit 'r'\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  __asm __emit 'd'\
  __asm __emit 'y'\
  __asm __emit 'm'\
  __asm __emit 'l'\
  __asm __emit 'c'\
  __asm __emit 'k'\
  __asm __emit '_'\
  __asm __emit 'b'\
  __asm __emit 'e'\
  __asm __emit 'g'\
  __asm __emit 'i'\
  __asm __emit 'n'\
  __asm __emit 0x61

 #define DYNAMIC_END \
  __asm __emit 0x60 \
  __asm __emit 0xeb \
  __asm __emit 58 \
  __asm __emit 'd'\
  __asm __emit 'y'\
  __asm __emit 'm'\
  __asm __emit 'l'\
  __asm __emit 'c'\
  __asm __emit 'k'\
  __asm __emit '_'\
  __asm __emit 'e'\
  __asm __emit 'n'\
  __asm __emit 'd'\
  twentyzero\
  twentyzero\
  fourzero \
  fourzero \
  __asm __emit 0x61


#define fourzero \
  __asm __emit 0\
  __asm __emit 0\
  __asm __emit 0\
  __asm __emit 0\
  
#define twentyzero \
    fourzero\
    fourzero\
    fourzero\
    fourzero\
    fourzero

#define hundredzero \
  twentyzero\
  twentyzero\
  twentyzero\
  twentyzero\
  twentyzero

#define kilozero \
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero

#define wanzero \
  kilozero\
  kilozero\
  kilozero\
  kilozero\
  kilozero\
  kilozero\
  kilozero\
  kilozero\
  kilozero\
  kilozero

#define embedded_hole \
  wanzero\
  kilozero\
  kilozero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero\
  hundredzero
  