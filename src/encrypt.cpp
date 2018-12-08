#include "encrypt.h"

Encrypt::Encrypt()
{
	SetOperation(AES_ENCRYPT);
}

void Encrypt::mixColumns(byte* State)
{
    byte tmp[4];
    int row;

	for (row = 0; row < 4; ++row)
	{
		tmp[0] = GM2[State[row*4 + 0]] ^ GM3[State[row*4 + 1]] ^ State[row*4 + 2] ^ State[row*4 + 3];
		tmp[1] = State[row*4 + 0] ^ GM2[State[row*4 + 1]] ^ GM3[State[row*4 +2]] ^ State[row*4 + 3];
		tmp[2] = State[row*4 + 0] ^ State[row*4 + 1] ^ GM2[State[row*4 + 2]] ^ GM3[State[row*4 + 3]];
		tmp[3] = GM3[State[row*4 + 0]] ^ State[row*4 + 1] ^ State[row*4 + 2] ^ GM2[State[row*4 + 3]];
		/*Asignamos tmp al State*/
		for (int col = 0; col < 4; ++col)
		{
			State[row*4 + col] = tmp[col];
		}
	}
}

void Encrypt::ExecuteAES(byte* State)
{
	TRACE("Input = %s", State);
	int round_it;
    AddRoundKey(State, 0);
    for (round_it = 1; round_it < getNumRound(); round_it++)
    {
        SubBytes(State);
        ShiftRows(State);
        mixColumns(State);
        AddRoundKey(State, round_it);
    }
    SubBytes(State);
    ShiftRows(State);
    AddRoundKey(State, round_it);
}

Encrypt::~Encrypt()
{

}
