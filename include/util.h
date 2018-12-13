#ifndef __UTIL__
#define __UTIL__

typedef unsigned char byte;

void xorblock16(void* block1, void* block2);
void hexStrToBinaryStr(byte* hexString, byte* binaryString, int length);

#endif /*__UTIL__*/
