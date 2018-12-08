#include "configure.h"
#include "encrypt.h"
#include "decrypt.h"
#include <stdio.h>

int main(void)
{
	TRACE("Encrypt");
	Encrypt* Ept = new Encrypt();
	Ept->SetKeySize(256);
	byte key[] = "abcdefgh01234567abcdefgh01234567";
	Ept->KeyExpansion(key);
	byte State[] =  "1234567887654321";
	Ept->ExecuteAES(State);
	TRACE_STATE(State);
	delete Ept;

	TRACE("Decrypt");
	Decrypt* Dpt = new Decrypt();
	Dpt->SetKeySize(256);
	Dpt->KeyExpansion(key);
	Dpt->ExecuteAES(State);
	TRACE_STATE(State);
	TRACE("%s", State);
	delete Dpt;

	return 0;
}
