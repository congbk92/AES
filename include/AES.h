#ifndef __AES_H__
#define __AES_H__

#include "configure.h"

enum enAlgorithmType
{
	AES_ENCRYPT,
	AES_DECRIPT
};

class AES
{
public:
	AES();
	void AddRoundKey(byte State[4][4], byte round);
	void SubBytes(byte State[4][4]);
	void ShiftRows(byte State[4][4], byte inversa);
	virtual void MixColumns(byte State[4][4]) = 0;
	virtual void executeAES(byte State[4][4], byte operation) = 0;
	virtual void setOperation() = 0;
	~AES();
private:
	int _operation;
	byte* _box;
};

#endif /* __AES_H__ */
