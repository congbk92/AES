#ifndef __DECRYPT_H__
#define __DECRYPT_H__

#include "configure.h"
#include "AES.h"

class Decrypt: public AES
{
public:
	Decrypt();
	virtual void MixColumns(byte State[4][4]);
	virtual void setOperation();
	virtual void executeAES(byte State[4][4], byte operation);
	~Decrypt();
};


#endif /* __DECRYPT_H__ */
