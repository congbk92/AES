#include "decrypt.h"

Decrypt::Decrypt()
{
	SetOperation(AES_DECRIPT);
}

void Decrypt::mixColumns(byte* State)
{
	byte tmp[4];
	int row;
	for (row = 0; row < 4; ++row)
	{
		tmp[0] = GM14[State[row*4 + 0]] ^ GM11[State[row*4 + 1]] ^ GM13[State[row*4 + 2]] ^ GM9[State[row*4 + 3]];
		tmp[1] = GM9[State[row*4 + 0]] ^ GM14[State[row*4 + 1]] ^ GM11[State[row*4 + 2]] ^ GM13[State[row*4 + 3]];
		tmp[2] = GM13[State[row*4 + 0]] ^ GM9[State[row*4 + 1]] ^ GM14[State[row*4 + 2]] ^ GM11[State[row*4 + 3]];
		tmp[3] = GM11[State[row*4 + 0]] ^ GM13[State[row*4 + 1]] ^ GM9[State[row*4 + 2]] ^ GM14[State[row*4 + 3]];
		/*Asignamos tmp al State*/
		for (int col = 0; col < 4; ++col)
		{
			State[row*4 + col] = tmp[col];
		}
	}
}

void Decrypt::ExecuteAES(byte* State)
{
    AddRoundKey(State, getNumRound());
    ShiftRows(State);
    SubBytes(State);
    for (byte round_it = getNumRound() - 1; round_it > 0; round_it--)
    {
        AddRoundKey(State, round_it);
        mixColumns(State);
        ShiftRows(State);
        SubBytes(State);
    }
    AddRoundKey(State, 0);
}

Decrypt::~Decrypt()
{

}
