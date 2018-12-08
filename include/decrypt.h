#ifndef __DECRYPT_H__
#define __DECRYPT_H__

#include "configure.h"
#include "AES.h"

class Decrypt: public AES
{
public:
	Decrypt();
	void MixColumns(byte* State);
	void ExecuteAES(byte* State);
	~Decrypt();
};


#endif /* __DECRYPT_H__ */
