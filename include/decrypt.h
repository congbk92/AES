#ifndef __DECRYPT_H__
#define __DECRYPT_H__

#include "configure.h"
#include "AES.h"

class Decrypt: public AES
{
public:
	Decrypt();
	void ExecuteAES(byte* State);
	~Decrypt();
private:
	void mixColumns(byte* State);
};


#endif /* __DECRYPT_H__ */
