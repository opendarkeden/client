#ifndef _CAVI_
#define _CAVI_

/* void Cls_OnMcinotify(HWND hwnd) // MM_MCINOTIFY */
//#define HANDLE_MM_MCINOTIFY(hwnd, wParam, lParam, fn) \
    //((fn)(hwnd, (LONG)lParam), 0L)

class CAVI{
public:
	HWND hwnd;
	DWORD dwID;
	BYTE bEndFlag;

	CAVI();
	//CAVI(HWND hwnd, LPSTR szName, int w=320, int h=240);

	int OpenMPG(HWND hwnd, LPSTR szName, int w=320, int h=240);
	int OpenAVI(HWND hwnd, LPSTR szName, int w=320, int h=240);
	void Close();
	int Play();
	void Stop();
};

#endif