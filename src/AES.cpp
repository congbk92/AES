#include "AES.h"

AES::AES()
{
	_operation = NONE;
	_box = NULL;
	_keySize = 0;
	_numRound = 0;
	_keyExp = 0;
}
void AES::SetOperation(enAlgorithmType algorithmType)
{
	_operation = algorithmType;
	if ( _operation == AES_ENCRYPT)
	{
		_box = SBOX;
	}
	else if ( _operation == AES_DECRIPT)
	{
		_box = INV_SBOX;
	}
	else
	{
		TRACE("algorithmType = %d", algorithmType);
		_box = NULL;
	}
}
void AES::AddRoundKey(byte* State, byte round)
{
    TRACE("Round %d", round);
    TRACE("Input");
	TRACE_STATE(State);
	if (State == NULL)
	{
		TRACE("State is NULL");
	}
	else
	{
	    int st_f, st_c;
	    for (st_f = 0; st_f < 4; st_f++)
	    {
	        for (st_c = 0; st_c < 4; st_c++)
	        {
	            State[st_f*4 + st_c] ^= _expandKey[round * 4 + st_f][st_c];
	        }
	    }
	    TRACE("Output");
		TRACE_STATE(State);
	}
}

void AES::SubBytes(byte* State)
{
	if(State == NULL || _box == NULL)
	{
		TRACE("State or _box is NULL");
	}
	else
	{
	    int state_f, state_c, sbox_f, sbox_c;
	    for (state_f = 0; state_f < 4; state_f++)
	    {
	        for (state_c = 0; state_c < 4; state_c++)
	        {
	            sbox_f = (State[state_f*4 + state_c]&0xf0) >> 4;
	            sbox_c = State[state_f*4 + state_c]&0x0f;
	            State[state_f*4 + state_c] = _box[sbox_f * 16 + sbox_c];
	        }
	     }
	}
}

void AES::ShiftRows(byte* State)
{
	if (State == NULL)
	{
		TRACE("State is NULL");
		return;
	}
    int state_col, w_it;
    byte w[4];

    for (state_col = 1; state_col < 4; ++state_col)
    {
        for (w_it = 0; w_it < 4; ++w_it)
		{
        	w[w_it] = State[w_it*4 + state_col];
		}
        RotateWord(w, state_col);
        for (w_it = 0; w_it < 4; ++w_it)
        {
        	State[w_it*4 + state_col] = w[w_it];
        }
    }
}

void AES::RotateWord(byte* wordToRotate, byte positions)
{
	if (wordToRotate == NULL)
	{
		TRACE("wordToRotate is NULL");
		return;
	}
    byte origWord[4] = {wordToRotate[0], wordToRotate[1], wordToRotate[2], wordToRotate[3]};
    int word_it, origWord_it;
    for (word_it = 0; word_it < 4; word_it++)
    {
        if (_operation == AES_ENCRYPT)
        {
            // Rotating to the left
            wordToRotate[word_it] = origWord[(word_it + positions) % 4];
        }
        else if (_operation == AES_DECRIPT)
        {
            // Rotation to the right
            origWord_it = ((word_it + positions * (-1)) % 4);
            wordToRotate[word_it] = origWord[(origWord_it < 0) ? origWord_it + 4 : origWord_it];
        }
    }
}

void AES::rotateWordForKeyExpansion(byte* wordToRotate, byte positions)
{
	if (wordToRotate == NULL)
	{
		TRACE("wordToRotate is NULL");
		return;
	}
    byte origWord[4] = {wordToRotate[0], wordToRotate[1], wordToRotate[2], wordToRotate[3]};
    int word_it;
    for (word_it = 0; word_it < 4; word_it++)
    {
		// Rotating to the left
		wordToRotate[word_it] = origWord[(word_it + positions) % 4];
    }
}

void AES::InitAES(byte* key, int keySize)
{
	setKeySize(keySize);
	keyExpansion(key);
}

void AES::keyExpansion(byte * Key)
{
	if (Key == NULL)
	{
		TRACE("Key is NULL");
		return;
	}
    byte temp[4];
    int word_it;

    /* Copy the first N bytes from the original key.
     * 16 for 128-bit keys, 24 for 192-bit keys, and 32 for 256-bit keys*/
    memcpy(_expandKey, Key, _keySize / _keyExp);

    /* Calculate the rest of the words for the expanded key*/
    for (int Round_it = _keyExp; Round_it < (_numRound + 1)*4; Round_it++)
    {
        // Get the last created word
        memcpy(temp, _expandKey[Round_it - 1], 4);
        if (Round_it % _keyExp == 0)
        {
            /* Each 8 words, rotate the word and apply the SBOX */
        	rotateWordForKeyExpansion(temp, 1);
            for (word_it = 0; word_it < 4; ++word_it)
            {
            	temp[word_it] = SBOX[(((temp[word_it]&0xf0) >> 4)*16)+(temp[word_it]&0x0f)];
            }
            /*XOR of the first element with the corresponding RCON value */
            temp[0] ^= Rcon[Round_it / _keyExp];
        }
        else if (_keyExp == 8 && Round_it % _keyExp == 4)
        {
            // This only applies to the 256-bit key length
            // It only applies the SBOX
            for (word_it = 0; word_it < 4; word_it++)
            {
            	temp[word_it] = SBOX[(((temp[word_it]&0xf0) >> 4)*16)+(temp[word_it]&0x0f)];
            }
        }
        /* For all the words, aply an XOR with the block N bytes before the current one */
        for (word_it = 0; word_it < 4; word_it++) temp[word_it] ^= _expandKey[Round_it - _keyExp][word_it];
        // Store the new word on th expanded key
        memcpy(_expandKey[Round_it], temp, 4);
    }

    //for debug only
    for(int i = 0; i < (_numRound + 1); i++)
    {
    	byte Key[16];
    	memcpy(Key, _expandKey[i*4], 16);
    	TRACE_STATE(Key);
    }

}

void AES::setKeySize(int keySize)
{
	_keySize = keySize;
	if(_keySize == 128)
	{
		_numRound = 10;
		_keyExp = 4;
	}
	else if(_keySize == 196)
	{
		_numRound = 12;
		_keyExp = 6;
	}
	else if(_keySize == 256)
	{
		_numRound = 14;
		_keyExp = 8;
	}
	else
	{
		TRACE("keySize = %d", keySize);
		_numRound = 0;
		_keyExp = 0;
	}
}

int AES::getNumRound()
{
	return _numRound;
}

AES::~AES()
{

}
