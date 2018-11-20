#ifndef __ENCRYPT_H__
#define __ENCRYPT_H__

#include "configure.h"
#include "AES.h"

class Encrypt: public AES
{
public:
	Encrypt();
	void MixColumns(byte* State);
	void ExecuteAES(byte* State);
	~Encrypt();
};

#endif /* __ENCRYPT_H__ */
