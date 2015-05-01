#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

static unsigned int g_seed;

inline void fast_srand(){
	g_seed = __rdtsc();
}

inline int fastrand(){
	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

inline UINT8 fastrand8(){
	return fastrand() & 0xFF;
}

inline UINT16 fastrand16(){
	return (fastrand8() << 8 | fastrand8());
}

inline UINT32 fastrand32(){
	return (fastrand16() << 16 | fastrand16());
}

inline UINT64 fastrand64(){
	return ((fastrand32() << 16) << 16 | fastrand32());
}

inline void fastrandbuffer(UINT8* buffer, int size){
	for (int ind = 0; ind < size; ind++){
		buffer[ind] = fastrand8();
	}
}