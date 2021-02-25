#define NPFGM_NT "npfgmNT.dll"
#define NPFGM_9X "npfgm9x.dll"

typedef bool(*ReleaseProtect)();
typedef bool(*EnterProtect)();
typedef bool(*nPFindSICE)();

ReleaseProtect pReleaseProtect;
EnterProtect pEnterProtect;
nPFindSICE pnPFindSICE;
