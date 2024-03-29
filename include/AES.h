#ifndef __AES_H__
#define __AES_H__

#include "configure.h"
#include "util.h"

enum enAlgorithmType
{
	NONE,
	AES_ENCRYPT,
	AES_DECRIPT
};

class AES
{
public:
	AES();
	virtual void ExecuteAES(byte* State) = 0;
	void InitAES(byte* key, int keySize);
	virtual ~AES();
protected:
	void AddRoundKey(byte* State, byte round);
	void SubBytes(byte* State);
	void ShiftRows(byte* State);
	virtual void mixColumns(byte* State) = 0;
	void SetOperation(enAlgorithmType algorithmType);
	void RotateWord(byte* wordToRotate, byte positions);
	int getNumRound();
private:
	void keyExpansion(byte * Key);
	void setKeySize(int keySize);
	void rotateWordForKeyExpansion(byte* wordToRotate, byte positions);
	int _operation;
	const byte* _box;
	byte _expandKey[60][4];
	// Key size
	int _keySize;		//128, 192 or 256
	// Number of rounds the AES will execute
	int _numRound;		// 128 -> 10, 192 -> 12, 256 -> 14
	// Key length (as a power of 2)
	int _keyExp;		// 128 -> 4, 192 -> 6, 256 -> 8
};

#endif /* __AES_H__ */
