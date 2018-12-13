#include "util.h"
#include <limits.h>
#include <stdio.h>

#if UINT_MAX == 18446744073709551615ULL
typedef unsigned int big64_t;
#elif ULONG_MAX == 18446744073709551615ULL
typedef unsigned long big64_t ;
#elif ULLONG_MAX == 18446744073709551615ULL
typedef unsigned long long big64_t;
#else
#error "Cannot find unsigned 64bit integer."
#endif


void xorblock16(void* block1, void* block2)		//result save to block1
{
	big64_t* tmp1 = reinterpret_cast<big64_t *>(block1);
	big64_t* tmp2 = reinterpret_cast<big64_t *>(block2);

	tmp1[0] = tmp1[0]^tmp2[0];
	tmp1[1] = tmp1[1]^tmp2[1];
}

void hexStrToBinaryStr(byte* hexString, byte* binaryString, int length)
{
	for (int i = 0; i < length - 1; i = i+2)
	{
		unsigned int tmp = 0;
		sscanf((const char*)(hexString + i),"%2x", &tmp);
		binaryString[i/2] = (byte) tmp;
	}
	return;
}
