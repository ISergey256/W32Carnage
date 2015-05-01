#include "stdafx.h"
#include "windows.h"
#include "BaseTsd.h"
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "syscalls.h"
#include "Proc.h"
#include "Rnd.h"

#define MAX_THREADS 16
#define NUM_THREADS 1

extern "C" {
	UINT64 syscallNumber;
	UINT64 syscallArgv[8];
	UINT64 syscallRet;
	UINT8 fakeStack[1024 * 1024];
	void  asmSyscall(void);
}

struct syscall_t *curSyscall[MAX_THREADS];
UINT32 threadHB[MAX_THREADS];

UINT64 _syscall(UINT64 number,
	UINT64 arg0,
	UINT64 arg1,
	UINT64 arg2,
	UINT64 arg3,
	UINT64 arg4,
	UINT64 arg5,
	UINT64 arg6,
	UINT64 arg7) {
	syscallNumber = number;
	syscallArgv[0] = arg0;
	syscallArgv[1] = arg1;
	syscallArgv[2] = arg2;
	syscallArgv[3] = arg3;
	syscallArgv[4] = arg4;
	syscallArgv[5] = arg5;
	syscallArgv[6] = arg6;
	syscallArgv[7] = arg7;
	try {
		asmSyscall();
	}
	catch (...) {
		printf("...\n");
	}
	return (syscallRet & 0xFFFFFFFF);
}

UINT64 getArg(struct syscall_t *curSyscall, int argIndex){
	if (curSyscall->argopt[argIndex] == __OPT){ //TODO: RAND
		//printf("Optional !\n");
		return 0; //NULL
	}

	switch (curSyscall->argtype[argIndex])
	{
	case __HWND :
		return get__HWND();
	case __DWORD :
	case __COLORREF:
	case __WCHAR:
		return get__DWORD();
	case __LPMSG:
		return get__LPMSG();
	case __WPARAM:
		return get__WPARAM();
	case __LPARAM:
		return get__LPARAM();
	case __BOOL:
		return get__BOOL();
	case __DWORD_PTR:
		return get__DWORD_PTR();
	case __LPDEVMODEW:
		return get__LPDEVMODEW();
	case __RECT:
	case __LPRECT:
	case __LPCRECT:
		return get__RECT();
	case __HDESK:
		return get__HDESK();
	case __HACCEL:
		return get__HACCEL();
	case __ACCESS_MASK:
		return get__ACCESS_MASK();
	case __HMENU :
		return get__HMENU();
	case __HINSTANCE:
	case __HMODULE:
		return get__HINSTANCE();
	case __HDWP:
		return get__HDWP();
	case __PPAINTSTRUCT:
		return get__PPAINTSTRUCT();
	case __WINDOWPLACEMENT:
		return get__WINDOWPLACEMENT();
	case __BLENDFUNCTION:
		return get__BLENDFUNCTION();
	case __PFLASHWINFO:
		return get__PFLASHWINFO();
	case __HICON:
		return get__HICON();
	case __HFONT:
		return get__HFONT();
	case __HCURSOR:
		return get__HCURSOR();
	case __PCURSORINFO:
		return get__PCURSORINFO();
	case __PICONINFO:
		return get__PICONINFO();
	case __PTITLEBARINFO:
		return get__PTITLEBARINFO();
	case __PALTTABINFO:
		return get__PALTTABINFO();
	case __HPALETTE:
		return get__HPALETTE();
	case __INT:
	case __UINT:
	case __ULONG:
	case __LONG:
		return get__INT();
	case __PUNICODE_STRING:
		return get__PUNICODE_STRING();
	case __PVOID:
	case __LPVOID:
		return get__PVOID();
	case __HBITMAP:
		return get__HBITMAP();
	case __HANDLE:
		return get__HANDLE();
	case __POBJECT_ATTRIBUTES:
		return get__POBJECT_ATTRIBUTES();
	case __HDC:
		return get__HDC();
	case __HRAWINPUT:
		return get__HRAWINPUT();
	case __WORD:
		return fastrand16();
	case __PUINT:
	case __PDWORD:
	case __LPDWORD:
	case __UINT_PTR:
		return get__PUINT();
	case __PRAWINPUT:
		return get__PRAWINPUT();
	case __LPPOINT:
	case __POINT:
		return get__PPOINT();
	case __HRGN:
		return get__HRGN();
	case __LPSCROLLINFO:
	case __LPCSCROLLINFO:
		return get__SCROLLINFO();
	case __PWNDCLASSEXW:
		return get__WNDCLASSEXW();
	case __PSCROLLBARINFO:
		return get__SCROLLBARINFO();
	case __LPWSTR:
		return get__PWSTR();
	case __PRAWINPUTDEVICE:
		return get__RAWINPUTDEVICE();
	case __BYTE:
		return fastrand8();
	case __PBYTE:
	case __LPBYTE:
		return get__PBYTE();
	case __HKL:
		return get__HKL();
	case __LPMOUSEMOVEPOINT:
		return  get__MOUSEMOVEPOINT();
	case __LPINPUT:
		return get__INPUT();
	case __LPTRACKMOUSEEVENT:
		return get__TRACKMOUSEEVENT();


	case __UNKNOWN:
	case __TIMERPROC:
	case __PCURSORDATA:
	case __USERTHREADINFOCLASS:
	case __PSBDATA:
	case __HOOKPROC:
	case __HHOOK:
	case __WINEVENTPROC:
	case __PPFNCLIENT:
	case __PPFNCLIENTWORKER:
		return get__UNKNOWN();
	default:
		//printf("[DEBUG] Unknow type %d \n", curSyscall->argtype[argIndex]);
		return get__UNKNOWN(); //TODO
	}
	return 0;
}

void fuzz(struct syscall_t *curSyscall){
	//printf("%s\n", curSyscall->name);
	UINT64 ret = 0xDEADBEEFDEADBEEF;

	UINT64 args[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	try{
		for (int i = 0; i < curSyscall->argn; i++){
			args[i] = getArg(curSyscall, i);
		}
		ret = _syscall(curSyscall->syscallnumber,
			args[0],
			args[1],
			args[2],
			args[3],
			args[4],
			args[5],
			args[6],
			args[7]);
	}
	catch (...){
		printf("...\n");
		return ;
	}
	//printf("ret %p\n", ret);
}

DWORD WINAPI fuzzThread(LPVOID lpParam) {
	UINT32 threadId = *((UINT32*)lpParam);
	printf("Go ! Fuzzer %d\n", threadId);
	try{
		while (1){
			int i = fastrand16() & 0xFFF; //TODO: do it better ! faster ! stronger !
			if (syscall_list[i].enabled == 1){
				threadHB[threadId]++;
				curSyscall[threadId] = &syscall_list[i];
				fuzz(curSyscall[threadId]);
			}
		}
	}
	catch (...){
		printf("...");
	}
	return 0;
}



int main(int argc, _TCHAR* argv[])
{
	fast_srand();
	LoadLibraryW(L"user32.dll");
	LoadLibraryW(L"gdi32.dll");

	//TODO: tests !
	/*printf("%p\n", get__HWND());
	//get__LPMSG();
	get__HDESK();
	get__HACCEL();
	get__HMENU();
	get__HDWP();
	get__HICON();
	get__HFONT();
	get__HCURSOR();
	getchar();*/

	HANDLE threadHandles[8];
	UINT32 threadNumber;
	UINT32 threadArg[MAX_THREADS];
	printf("Starting Fuzzing threads...\n");
	for (threadNumber = 0; threadNumber < NUM_THREADS; threadNumber++){
		printf("Starting thread %d\n", threadNumber);
		threadArg[threadNumber] = threadNumber;
		threadHandles[threadNumber] = CreateThread(NULL, 0, fuzzThread, &threadArg[threadNumber], 0, NULL);
	}

	Sleep(1000);

	printf("Starting watchdog...\n");
	while (1) {
		UINT64 totalTicks = 0;
		for (UINT32 i = 0; i < threadNumber; i++) {
			if (threadHB[i] == 0) {
				//Terminate the thread
				TerminateThread(threadHandles[i], 0);
				//Black-list this syscall
				curSyscall[i]->enabled = 0;
				printf("%s blacklisted with restart of %d !\n", curSyscall[i]->name, i);
				//Re-create the thread
				threadHandles[i] = CreateThread(NULL, 0, fuzzThread, &threadArg[i], 0, NULL);
				threadHB[i] = 1;
			}else{
				totalTicks += threadHB[i];
				threadHB[i] = 0;
			}
		}
		Sleep(1000);
		printf("~%d syscalls/s\n", totalTicks);
	}
	return 0;

	return 0;
}




INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	//OutputDebugStringW(L"[DEBUG] Go Fuzz !%s\n", "toto");

	main(0, NULL);

	return 0;
}