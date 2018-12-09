#include "configure.h"
#include "encrypt.h"
#include "decrypt.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	/*
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
	*/
	//ebc mode without salt, md sha1
//	if()

	if (argc != 6)
	{
		std::cout<<argc;
		std::cout<<"Number of pass argument should be 5 and follow the below format:\n";
		std::cout<<"#type AES (enc_ecb, dec_ecb, enc_cbc, dec_cbc) #key length (128,192,256) #pass #input file #output file\n";
		return 0;
	}
	else
	{
		std::string type_aes = argv[1];
		std::string sKeyLenght = argv[2];
		int keyLenght = atoi(argv[2]);
		std::string key = argv[3];
		std::string inputFileName = argv[4];
		std::string outputFileName = argv[5];

		if(type_aes != "enc_ecb" && type_aes != "dec_ecb" && type_aes != "enc_cbc" && type_aes != "dec_cbc")
		{
			std::cout<<"Invalid type of AES\n";
			std::cout<<"Type of AES should be enc_ecb, dec_ecb, enc_cbc or dec_cbc\n";
			return 0;
		}

		if(sKeyLenght != "128" && sKeyLenght != "192" && sKeyLenght != "256")
		{
			std::cout<<"Invalid key length\n";
			std::cout<<"key length should be 128, 192, 256\n";
			return 0;
		}

		std::ifstream f(inputFileName.c_str());
		if(!f.good())
		{
			std::cout<<"The input file is not exist\n";
			f.close();
			return 0;
		}
		f.close();

		// no salt, no key-derivation function, padding is PKCS#7
		if(type_aes == "enc_ecb")
		{
			Encrypt* Ept = new Encrypt();
			Ept->InitAES((byte*)key.c_str(),keyLenght);

			std::ifstream inputFile;
			inputFile.open(inputFileName, std::ios::binary);

			byte State[16];

			std::ofstream outputFile;
			outputFile.open (outputFileName, std::ofstream::out);

			while(true)
			{
				int extracted = inputFile.read((char*)State,16).gcount();
				if (extracted < 16)	//padding is PKCS#7
				{
					byte paddingValue = 16 - extracted;
					//std::cout<<(int)paddingValue<<"\n";
					for (byte i = 16 - paddingValue; i < 16; i++)
					{
						State[i] = paddingValue;
					}
					Ept->ExecuteAES(State);
					outputFile.write((const char*)State,16);
					break;
				}
				Ept->ExecuteAES(State);
				outputFile.write((const char*)State,16);
			}
			outputFile.close();
			inputFile.close();
		}
		else if(type_aes == "dec_ecb")
		{
			Decrypt* Dpt = new Decrypt();
			Dpt->InitAES((byte*)key.c_str(),keyLenght);

			std::ifstream inputFile;
			inputFile.open(inputFileName, std::ios::binary);

			byte State[16];

			std::ofstream outputFile;
			outputFile.open (outputFileName, std::ofstream::out);
			while(inputFile.peek()!=EOF)
			{
				inputFile.read((char*)State,16);
				Dpt->ExecuteAES(State);
				if (inputFile.peek() == EOF)		//removed padding
				{
					if(State[15] < 16)
					{
						outputFile.write((const char*)State,16 - State[15]);
					}
				}
				else
				{
					outputFile.write((const char*)State,16);
				}
			}
			inputFile.close();
			outputFile.close();
		}
	}

	return 0;
}
