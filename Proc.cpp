#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "Rnd.h"



LRESULT CALLBACK WindowProcedure(HWND window, unsigned int msg, WPARAM wp, LPARAM lp)
{
	return 0;
}

HWND cur__HWND;
UINT8 check__HWND(HWND window){
	//if(...){
	//	return 0;
	//}
	return 0;
}

HWND create__HWND(){
	cur__HWND = GetConsoleWindow();
	return cur__HWND;

	//TODO: Create Window !
	HINSTANCE hInstance = GetModuleHandle(NULL);
	printf("hInstance %p\n", hInstance);
	cur__HWND = CreateWindow(TEXT("Edit"), TEXT("test"), WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 20, 140, 20, NULL, NULL, NULL, NULL);

	LPCWSTR myclass = L"myclass";
	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, WindowProcedure,
		0, 0, GetModuleHandle(0), LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
		0, myclass, LoadIcon(0, IDI_APPLICATION) };
	if (RegisterClassEx(&wndclass))
	{
		printf("1\n");
		cur__HWND = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			myclass,
			L"The title of my window",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
			NULL, NULL, hInstance, NULL);
		if (cur__HWND){
			return cur__HWND;
		}
	}
	printf("create__HWND failed !\n");
	return NULL;
}


UINT64 get__HWND(){
	if (check__HWND(cur__HWND) == 0){
		cur__HWND = create__HWND();
	}
	return (UINT64)&cur__HWND;
}



UINT64 get__DWORD(){
	return fastrand32();
}

MSG cur__LPMSG;
UINT64 get__LPMSG(){
	/*if (GetMessage(&cur__LPMSG, cur__HWND, 0, 0) == -1){
		printf("GetMessage Error !\n");
	}*/
	return (UINT64)&cur__LPMSG;
}

UINT64 get__WPARAM(){
	return fastrand64();
}

UINT64 get__LPARAM(){
	return fastrand64();
}

UINT64 get__BOOL(){
	if (fastrand8() & 0x1){
		return 1;
	}
	return 0;
}

UINT32 cur__DWORD_PTR;
UINT64 get__DWORD_PTR(){
	cur__DWORD_PTR = fastrand32();
	return (UINT64)&cur__DWORD_PTR;
}

typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;
UNICODE_STRING cur__PUNICODE_STRING;

UINT64 get__PUNICODE_STRING(){ //TODO: better rand :)
	cur__PUNICODE_STRING.Length = fastrand16();
	cur__PUNICODE_STRING.MaximumLength = fastrand16();
	cur__PUNICODE_STRING.Buffer = L"sqmdlkjf";
	return (UINT64)&cur__PUNICODE_STRING;
}

DEVMODEW cur__LPDEVMODEW;
UINT64 get__LPDEVMODEW(){
	return (UINT64)&cur__LPDEVMODEW;
}


RECT cur__RECT;
UINT64 get__RECT(){
	cur__RECT.left = fastrand32();
	cur__RECT.top = fastrand32();
	cur__RECT.right = fastrand32();
	cur__RECT.bottom = fastrand32();
	return (UINT64)&cur__RECT;
}


HDESK cur__HDESK;
UINT64 get__HDESK(){
	//Make a new desktop
	cur__HDESK = CreateDesktop(_T("kaka"), 0, 0, 0, DESKTOP_SWITCHDESKTOP |
		DESKTOP_WRITEOBJECTS |
		DESKTOP_READOBJECTS |
		DESKTOP_ENUMERATE |
		DESKTOP_CREATEWINDOW |
		DESKTOP_CREATEMENU, NULL);
	if (cur__HDESK == NULL){
		printf("get__HDESK error !\n");
	}
	return (UINT64)&cur__HDESK;
}

HACCEL cur__HACCEL;
UINT64 get__HACCEL(){
	//TODO:
	cur__HACCEL = LoadAccelerators(NULL, L"FontAccel");
	if (cur__HACCEL == NULL){
		//printf("get__HACCEL error !\n");
	}
	return (UINT64)&cur__HACCEL;
}


/*LPACCEL lpAccel=new ACCEL[3];
lpAccel[0].cmd=ID_EXIT; //Fill Structures
lpAccel[0].fVirt=FALT;
lpAccel[0].key='q';
lpAccel[1].cmd=ID_SAVE;
lpAccel[1].fVirt=FALT;
lpAccel[1].key='s';
lpAccel[2].cmd=ID_CAPTION;
lpAccel[2].fVirt=FALT;
lpAccel[2].key='c';*/

HBITMAP cur__HBITMAP;
UINT8 tmpBuffer[1024];
UINT64 get__HBITMAP(){
	cur__HBITMAP = CreateBitmap(
		fastrand16(),
		fastrand16(),
		fastrand16(),
		fastrand16(),
		tmpBuffer);
	return (UINT64)&cur__HBITMAP;
}



UINT64 get__ACCESS_MASK(){
	return fastrand32();
}

HMENU cur__HMENU;
UINT64 get__HMENU(){
	cur__HMENU = GetMenu(cur__HWND);
	if (cur__HMENU == NULL){
		//TODO: 
		//printf("get__HMENU error !\n");
	}
	return (UINT64)&cur__HMENU;
}

HINSTANCE cur__HINSTANCE;
UINT64 get__HINSTANCE(){
	cur__HINSTANCE = GetModuleHandle(NULL);
	return (UINT64)&cur__HINSTANCE;
}

HDWP cur__HDWP;
UINT64 get__HDWP(){
	cur__HDWP = BeginDeferWindowPos(0);
	if (cur__HDWP == NULL){
		//printf("get__HDWP error !\n");
	}
	return (UINT64)&cur__HDWP;
}

PAINTSTRUCT cur__PPAINTSTRUCT;
UINT64 get__PPAINTSTRUCT(){
	return (UINT64)&cur__PPAINTSTRUCT;
}

WINDOWPLACEMENT  cur__WINDOWPLACEMENT;
UINT64 get__WINDOWPLACEMENT(){
	GetWindowPlacement(cur__HWND, &cur__WINDOWPLACEMENT);
	return (UINT64)&cur__WINDOWPLACEMENT;
}

BLENDFUNCTION cur__BLENDFUNCTION;
UINT64 get__BLENDFUNCTION(){
	return (UINT64)fastrand32();
}

FLASHWINFO cur__PFLASHWINFO;
UINT64 get__PFLASHWINFO(){
	cur__PFLASHWINFO.cbSize = fastrand32();
	//cur__FLASHWINFO.hwnd = cur__HDWP; //TODO:
	cur__PFLASHWINFO.dwFlags = fastrand32();
	cur__PFLASHWINFO.uCount = fastrand32();
	cur__PFLASHWINFO.dwTimeout = fastrand32();
	return (UINT64)&cur__PFLASHWINFO;
}

UINT64 get__COLORREF(){
	return (UINT64)fastrand32();
}

HICON cur__HICON;
UINT64 get__HICON(){
	cur__HICON = LoadIcon(NULL, IDI_QUESTION);
	if (cur__HICON == NULL){
		printf("get__HICON error !\n");
	}
	return (UINT64)cur__HICON;
}

HFONT cur__HFONT;
UINT64 get__HFONT(){
	cur__HFONT = CreateFont(46, 28, 215, 0,
		FW_NORMAL, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN,
		L"Times New Roman");
	if (cur__HFONT == NULL){
		printf("get__HFONT error !\n");
	}
	return (UINT64)&cur__HFONT;
}

HCURSOR cur__HCURSOR;
UINT64 get__HCURSOR(){
	cur__HCURSOR = LoadCursor(0, IDC_APPSTARTING);
	if (cur__HCURSOR == NULL){
		printf("get__HCURSOR error !\n");
	}
	return (UINT64)&cur__HCURSOR;
}

CURSORINFO curr__PCURSORINFO;
UINT64 get__PCURSORINFO(){
	GetCursorInfo(&curr__PCURSORINFO);
	return (UINT64)&curr__PCURSORINFO;
}


ICONINFO cur__PICONINFO;
UINT64 get__PICONINFO(){
	GetIconInfo(cur__HICON, &cur__PICONINFO);
	return (UINT64)&cur__PICONINFO;
}

/*PGETCLIPBDATA cur__GETCLIPBDATA;
UINT64 get__cur__GETCLIPBDATA(){
	cur__GETCLIPBDATA = GetClipboardData(fastrand32());
	return (UINT64)cur__GETCLIPBDATA;
*/

TITLEBARINFO cur__PTITLEBARINFO;
UINT64 get__PTITLEBARINFO(){
	GetTitleBarInfo(cur__HWND, &cur__PTITLEBARINFO);
	return (UINT64)&cur__PTITLEBARINFO;
}

ALTTABINFO cur__PALTTABINFO = {};
UINT64 get__PALTTABINFO(){
	cur__PALTTABINFO.cbSize = sizeof(ALTTABINFO);
	GetAltTabInfo(cur__HWND, -1, &cur__PALTTABINFO, NULL, 0);
	return (UINT64)&cur__PALTTABINFO;
}

HPALETTE cur__HPALETTE;
UINT64 get__HPALETTE(){
	/*int paletteSize = fastrand16();
	LOGPALETTE* pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
	pPal->palVersion = 0x300;
	pPal->palNumEntries = fastrand32();
	{
		int redMask = (1 << fastrand32()) - 1;
		int greenMask = (1 << fastrand32()) - 1;
		int blueMask = (1 << fastrand32()) - 1;
		int i;

			for (i = 0; i<paletteSize; ++i) {
			pPal->palPalEntry[i].peRed =
				(((i >> fastrand32()) & redMask) * 255) / redMask;
			pPal->palPalEntry[i].peGreen =
				(((i >> fastrand32()) & greenMask) * 255) / greenMask;
			pPal->palPalEntry[i].peBlue =
				(((i >> fastrand32()) & blueMask) * 255) / blueMask;
			pPal->palPalEntry[i].peFlags = 0;
		}
	}

	cur__HPALETTE = CreatePalette(pPal);
	free(pPal);*/
	return (UINT64)cur__HPALETTE;
}



#define BUFFER_SIZE 4*1024
UINT64 argBuffers[0xFF][BUFFER_SIZE];

UINT64 getRandomArg(){ //TODO:
	UINT8 bufferNo = 0;
	UINT8 state = (fastrand8() & 0xF);
	switch (state)
	{
	case 0:
		return 0;
	case 1:
		return fastrand8();
	case 2:
		return fastrand16();
	case 12:
		return (fastrand64() | 0xFFFF000000000000);
	case 13:
		return (UINT64)argBuffers[fastrand8()];
	case 14:
		return 0xFFFFFFFFFFFFFFFF;
	case 15:
		return fastrand64();
	default:
		return fastrand64();
	}
	return 0;
}


UINT64 get__UNKNOWN(){ //TODO:
	UINT8 bufferNo = 0;
	UINT8 state = (fastrand8() & 0xF);
	switch (state)
	{
	case 0:
		return 0;
	case 1:
		return fastrand8();
	case 2:
		return fastrand16();
	case 12:
		return (fastrand64() | 0xFFFF000000000000);
	case 13:
		bufferNo = fastrand8();
		for (int i = 0; i < BUFFER_SIZE / 8; i++){
			argBuffers[bufferNo][i] = getRandomArg();
		}
		return (UINT64)argBuffers[bufferNo];
	case 14:
		return 0xFFFFFFFFFFFFFFFF;
	case 15:
		return fastrand64();
	default:
		return fastrand64();
	}
	return 0;
}

UINT64 get__INT(){
	return (UINT64)fastrand32();
}


UINT8 cur__PVOID[6 * 1024];
UINT64 get__PVOID(){ //TODO: Speed !
	for (int i = 0; i < 6 * 1024; i++){
		cur__PVOID[i] = fastrand8();
	}
	return (UINT64)cur__PVOID;
}

HANDLE cur__HANDLE;
UINT64 get__HANDLE(){
	return (UINT64)cur__HANDLE;
}

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
OBJECT_ATTRIBUTES cur__POBJECT_ATTRIBUTES;
UINT64 get__POBJECT_ATTRIBUTES(){
	//TODO:
	return (UINT64)&cur__POBJECT_ATTRIBUTES;
}
HDC cur__HDC;
UINT64 get__HDC(){
	cur__HDC =  GetDC(cur__HWND);
	return (UINT64)cur__HDC;
}

HRAWINPUT cur__HRAWINPUT;
UINT64 get__HRAWINPUT(){
	return (UINT64)cur__HRAWINPUT;
}

UINT cur__PUINT;
UINT64 get__PUINT(){
	cur__PUINT = fastrand32();
	return (UINT64)&cur__PUINT;
}

UINT64 rawBuffer[1024 / 8];
UINT32 bytes = sizeof(rawBuffer);
UINT64 get__PRAWINPUT(){
	GetRawInputBuffer((PRAWINPUT)rawBuffer, &bytes, sizeof(RAWINPUTHEADER));
	return (UINT64)rawBuffer;
}

POINT cur__POINT;
UINT64 get__PPOINT(){
	cur__POINT.x = fastrand64();
	cur__POINT.y = fastrand64();
	return (UINT64)&cur__POINT;
}

HRGN cur__HRGN;
UINT64 get__HRGN(){
	return (UINT64)cur__HRGN;
}

SCROLLINFO cur__SCROLLINFO;
UINT64 get__SCROLLINFO(){
	return (UINT64)&cur__SCROLLINFO;
}

WNDCLASSEXW cur__WNDCLASSEXW;
UINT64 get__WNDCLASSEXW(){
	//TODO !
	return (UINT64)&cur__WNDCLASSEXW;
}
 

SCROLLBARINFO cur__SCROLLBARINFO;
UINT64 get__SCROLLBARINFO(){
	//TODO: !
	return (UINT64)&cur__SCROLLBARINFO;
}

PWSTR cur__PWSTR;
UINT64 get__PWSTR(){
	cur__PWSTR = L"myclass"; //TODO: random
	return (UINT64)cur__PWSTR;
}

RAWINPUTDEVICE cur__RAWINPUTDEVICE;
UINT64 get__RAWINPUTDEVICE(){
	UINT nbDevice = 1;
	GetRegisteredRawInputDevices(&cur__RAWINPUTDEVICE, &nbDevice, sizeof(RAWINPUTDEVICE));
	return (UINT64)&cur__RAWINPUTDEVICE;
}


BYTE cur__BYTE;
UINT64 get__PBYTE(){
	cur__BYTE = fastrand8();
	return (UINT64)&cur__BYTE;
}

HKL cur__HKL;
UINT64 get__HKL(){
	cur__HKL = LoadKeyboardLayout(L"00000409", KLF_ACTIVATE);
	return (UINT64)cur__HKL;
}

MOUSEMOVEPOINT cur__MOUSEMOVEPOINT;
ULONG tmpULONG;
UINT64 get__MOUSEMOVEPOINT(){
	cur__MOUSEMOVEPOINT.x = fastrand32();
	cur__MOUSEMOVEPOINT.y = fastrand32();
	cur__MOUSEMOVEPOINT.time = fastrand32();
	tmpULONG = fastrand64();
	cur__MOUSEMOVEPOINT.dwExtraInfo = (ULONG_PTR)&tmpULONG;
	return (UINT64)&cur__MOUSEMOVEPOINT;
}



INPUT cur__INPUT;
UINT64 get__INPUT(){
	fastrandbuffer((UINT8*)&cur__INPUT, sizeof(INPUT));
	cur__INPUT.type = fastrand8() & 0x3;
	return (UINT64)&cur__INPUT;
}

TRACKMOUSEEVENT cur__TRACKMOUSEEVENT;
UINT64 get__TRACKMOUSEEVENT(){
	fastrandbuffer((UINT8*)&cur__TRACKMOUSEEVENT, sizeof(TRACKMOUSEEVENT));
	cur__TRACKMOUSEEVENT.hwndTrack = cur__HWND;
	return (UINT64)&cur__TRACKMOUSEEVENT;
}