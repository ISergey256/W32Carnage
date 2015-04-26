#include "stdafx.h"
#include "syscall.h"
#include "windows.h"
#include "BaseTsd.h"
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_THREADS 8
#define MAX_SYSCALLS 0x1FFF
#define W32K_FUZZING 1
#define STATUS_INVALID_SYSTEM_SERVICE 0x00000000C000001C
#define PRINT_FUZZ 0
#define BUFFER_SIZE 1024*8
#define EXTRA_ARGS 16
#define NUM_THREADS 4

extern "C" {
	UINT64 syscallNumber;
	UINT64 syscallArgv[8];
	UINT64 syscallRet;
	UINT8 fakeStack[1024 * 1024];
}

UINT8 threadHB[MAX_THREADS];
UINT8 allowedSyscall[MAX_SYSCALLS];
UINT64 lastSyscall[MAX_THREADS];
UINT64 argBuffers[EXTRA_ARGS][BUFFER_SIZE];
UINT64 tick[MAX_THREADS]; //Lock needed, but don't care...

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

inline UINT8 fastRand8() {
	return __rdtsc() & 0xFF;
}

UINT64 rand64() {
	UINT64 tmpRand = 0;
	int i;
	for (i = 0; i < 8; i++) {
		tmpRand = tmpRand | fastRand8();
		if (i != 7) {
			tmpRand = tmpRand << 8;
		}
	}
	return tmpRand;
}

UINT64 getRandomArg(INT8 fillBuffer){
	UINT8 bufferNo = 0;
	UINT8 state = (fastRand8() & 0xF);
	switch (state)
	{
	case 0:
		return 0;
	case 1:
		return fastRand8();
	case 2:
		return (fastRand8() << 8) | fastRand8();
	case 13:
		return (rand64() | 0xFFFF000000000000);
	case 14:
		bufferNo = fastRand8();
		if (fillBuffer > 1){
			int i;
			for (i = 0; i < BUFFER_SIZE / 8; i++){
				argBuffers[bufferNo][i] = getRandomArg(0);
			}
		}
		return (UINT64)argBuffers[bufferNo];
	case 15:
		return 0xFFFFFFFFFFFFFFFF;
	default:
		return rand64();
	}
	return 0;
}

DWORD WINAPI fuzzThread(LPVOID lpParam) {
	UINT32 threadId = *((UINT32*)lpParam);
	printf("Go ! Fuzzer %d\n", threadId);
	try{
		threadHB[threadId] = 1;
		UINT32 fuzzNumber = 1000000;
		UINT32 fuzzTries = 0;
		while (fuzzTries < fuzzNumber) {
			UINT64 rndSyscallNumber = (__rdtsc() & 0xFFF);
			if (W32K_FUZZING) {
				rndSyscallNumber = rndSyscallNumber + 0x1000;
			}
			if (allowedSyscall[rndSyscallNumber] == 1) {
				lastSyscall[threadId] = rndSyscallNumber;
				UINT64 arg0 = getRandomArg(1);
				UINT64 arg1 = getRandomArg(1);
				UINT64 arg2 = getRandomArg(1);
				UINT64 arg3 = getRandomArg(1);
				UINT64 arg4 = getRandomArg(1);
				UINT64 arg5 = getRandomArg(1);
				UINT64 arg6 = getRandomArg(1);
				UINT64 arg7 = getRandomArg(1);
				if (PRINT_FUZZ){
					printf("@%d %08x(%p, %p, %p, %p)...", fuzzTries, rndSyscallNumber, arg0, arg1, arg2, arg3);
				}
				UINT64 ret = _syscall(rndSyscallNumber, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				tick[threadId]++;
				if (ret == STATUS_INVALID_SYSTEM_SERVICE) {
					//allowedSyscall[rndSyscallNumber] = 0;
				}
				if (PRINT_FUZZ){
					printf("%p\n", ret);
				}
				fuzzTries++;
				threadHB[threadId] = 1;
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
	LoadLibraryW(L"user32.dll");
	LoadLibraryW(L"gdi32.dll");

	UINT32 zero = 0;

	UINT32 i;
	for (i = 0; i < MAX_THREADS; i++) {
		threadHB[i] = 0;
		tick[i] = 0;
	}
	for (i = 0; i < MAX_SYSCALLS; i++) {
		allowedSyscall[i] = 1;
	}

	HANDLE threadHandles[8];
	UINT32 threadNumber = 0;
	UINT32 threadArg[MAX_THREADS];
	printf("Starting Fuzzing threads...\n");
	for (i = 0; i < NUM_THREADS; i++){
		printf("Starting thread %d\n", threadNumber);
		threadArg[threadNumber] = threadNumber;
		threadHandles[threadNumber] = CreateThread(NULL, 0, fuzzThread, &threadArg[threadNumber], 0, NULL);
		printf("%p\n", threadHandles[threadNumber]);
		threadNumber++;
	}

	printf("Starting watchdog...\n");
	while (1) {
		UINT64 totalTicks = 0;
		for (i = 0; i < threadNumber; i++) {
			if (threadHB[i] == 0) {
				//Terminate the thread
				TerminateThread(threadHandles[i], 0);
				//Black-list this syscall
				allowedSyscall[lastSyscall[i]] = 0;
				printf("%08x blacklisted with restart of %d !\n", lastSyscall[i], i);
				//Re-create the thread
				threadHandles[i] = CreateThread(NULL, 0, fuzzThread, &threadArg[i], 0, NULL);
				threadHB[i] = 1;
			}else{
				threadHB[i] = 0;
			}
			totalTicks += tick[i];
			tick[i] = 0;
		}
		Sleep(1000);
		printf("~%d syscalls/s\n", totalTicks);
	}
	return 0;
}

