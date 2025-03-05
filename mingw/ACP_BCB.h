/* ACProtect*/
#define GetRegistrationName(szRegistrationName)  ::MessageBox (HWND(-1),szRegistrationName,NULL,0);
#define GetTrialUsageTimes(lpUsageTime)          ::MessageBox (HWND(-1),lpUsageTime       ,NULL,1);

#define EMBEDDED_BEGIN \
  __asm {db 0x60,0x6a,0x05,0x6a,0x00,0x6a,0x00,0x6a,0xff} \
  __asm {call RegisterHotKey}\
  __asm {db 0x61,0x60,0xe9,0x1c,0x32,0 ,0,'peetles__header'}\
  __asm {db 12800 dup(0)}\
  __asm {db 'peetles_begin',0x61}

 #define EMBEDDED_END \
  __asm {db 0x60,0x6a,0x04,0x6a,0x00,0x6a,0x00,0x6a,0xff} \
  __asm {call RegisterHotKey}\
  __asm {db 235, 30 ,'peetles_end'}\
  __asm {db 19 dup(0)}\
  __asm db 0x61

 #define RSALOCK_BEGIN \
  __asm {db 0x60,0x6a,0x02,0x6a,0x00,0x6a,0x00,0x6a,0xff} \
  __asm {call RegisterHotKey}\
  __asm {db 0xeb,0x0d,'RegOnly_begin',0x61}

  #define RSALOCK_END\
  __asm {db 0x60,0xeb,0x0b,'RegOnly_end',0x6a,0x03,0x6a,0x00,0x6a,0x00,0x6a,0xff} \
  __asm {call RegisterHotKey}\
  __asm db 0x61

   #define DYNAMIC_BEGIN \
  __asm { db 0x60,0xe9,0xaa,0x01,0,0,'dymlck__header'}\
  __asm { db 400 dup(0)}\
  __asm { db 'dymlck_begin',0x61}

 #define DYNAMIC_END \
  __asm {db 0x60,0xeb,58,'dymlck_end'}\
  __asm {db 48 dup(0)}\
  __asm {db 0x61}

