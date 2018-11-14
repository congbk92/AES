#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include "configure.h"
#include "AES.h"

class Encrypt: public AES
{
public:
	Encrypt();
	virtual void MixColumns(byte State[4][4]);
	virtual void setOperation();
	virtual void executeAES(byte State[4][4], byte operation);
	~Encrypt();
};

#endif /* __ENCRYPT_H__ */
