#include "configure.h"
#include "encrypt.h"

int main(void)
{
	Encrypt* Ept = new Encrypt();
	Ept->SetKeySize(128);
	byte key[16] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x0A};
	Ept->KeyExpansion(key);
	byte State[] =  "1234567812345678";
	//byte Buffer = "1234567812345678";
	//memcpy(State, Buffer, 16);
	TRACE("Input = %s", State);
	Ept->ExecuteAES(State);
	TRACE("Output = %s", State);
	delete Ept;
	return 0;
}
