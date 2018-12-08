#include "configure.h"
#include "encrypt.h"
#include "decrypt.h"
#include <stdio.h>

int main(void)
{
	TRACE("Encrypt");
	byte key[] = "abcdefgh01234567abcdefgh01234567";
	Encrypt* Ept = new Encrypt();
	Ept->InitAES(key,128);
	byte State[] =  "1234567887654321";
	Ept->ExecuteAES(State);
	TRACE_STATE(State);
	TRACE("%s", State);
	delete Ept;

	TRACE("Decrypt");
	Decrypt* Dpt = new Decrypt();
	Dpt->InitAES(key,128);
	Dpt->ExecuteAES(State);
	TRACE_STATE(State);
	TRACE("%s", State);
	delete Dpt;

	return 0;
}
