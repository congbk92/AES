#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include "configure.h"
#include "AES.h"

class Encrypt: public AES
{
public:
	Encrypt();
	void ExecuteAES(byte* State);
	~Encrypt();
private:
	void mixColumns(byte* State);
};

#endif /* __ENCRYPT_H__ */
