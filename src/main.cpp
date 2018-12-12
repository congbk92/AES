#include "configure.h"
#include "encrypt.h"
#include "decrypt.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

void xorblock16(byte* block1, byte* block2)		//result save to block1
{
	for(int i = 0; i<16; i++)
	{
		block1[i] = block1[i]^block2[i];
	}
}
int main(int argc, char** argv)
{
	while(true)
	{
		//Checking input
		std::cout<<"Please input follow the below format:\n";
		std::cout<<"#type_AES #key_length #file_key #input_file #output_file\n";
		std::cout<<"#type_AES: enc_ecb, dec_ecb, enc_cbc, dec_cbc\n";
		std::cout<<"#key_length: 128, 192, 256\n";
		std::cout<<"Or press Q to quit\n";

		std::string type_aes;
		int keyLenght;
		std::string key;
		std::string keyFile;
		std::string inputFileName;
		std::string outputFileName;

		if(!(std::cin>>type_aes>>keyLenght>>keyFile>>inputFileName>>outputFileName))
		{
			if (type_aes == "q")
			{
				break;
			}

			std::cin.clear();
			std::cin.ignore(10000,'\n');
			continue;
		}

		if(type_aes != "enc_ecb" && type_aes != "dec_ecb" && type_aes != "enc_cbc" && type_aes != "dec_cbc")
		{
			std::cout<<"Invalid type of AES\n";
			std::cout<<"Type of AES should be enc_ecb, dec_ecb, enc_cbc or dec_cbc\n";
			continue;
		}

		if(keyLenght != 128 && keyLenght != 192 && keyLenght != 256)
		{
			std::cout<<"Invalid key length\n";
			std::cout<<"key length should be 128, 192, 256\n";
			continue;
		}

		std::ifstream f(keyFile.c_str());
		if(!f.good())
		{
			std::cout<<"The key file is not exist\n";
			f.close();
			continue;
		}
		std::getline(f,key);
		f.close();

		f.open(inputFileName.c_str());
		if(!f.good())
		{
			std::cout<<"The input file is not exist\n";
			f.close();
			continue;
		}
		f.close();

		TRACE("type_AES: %s", type_aes.c_str());
		TRACE("key_length: %d", keyLenght);
		TRACE("key: %s", key.c_str());
		TRACE("input_file: %s", inputFileName);
		TRACE("output_file: %s", outputFileName);

		//begin encrypt or Decrypt
		// no salt, no key-derivation function, padding is PKCS#7
		if(type_aes == "enc_ecb")
		{
			Encrypt* Ept = new Encrypt();
			Ept->InitAES((byte*)key.c_str(),keyLenght);

			std::ifstream inputFile;
			inputFile.open(inputFileName, std::ios::binary);

			byte State[16];

			std::ofstream outputFile;
			outputFile.open (outputFileName, std::ofstream::out | std::ios::binary);
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
			delete Ept;
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
			delete Dpt;
		}
		else if(type_aes == "enc_cbc")
		{
			Encrypt* Ept = new Encrypt();
			Ept->InitAES((byte*)key.c_str(),keyLenght);

			std::ifstream inputFile;
			inputFile.open(inputFileName, std::ios::binary);

			byte State[16];

			std::ofstream outputFile;
			outputFile.open (outputFileName, std::ofstream::out);

			byte initVector[16];
			memset(initVector,0,sizeof(initVector));
			bool flagFinish = false;
			while(!flagFinish)
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
					flagFinish = true;
				}
				xorblock16(State,initVector);
				Ept->ExecuteAES(State);
				memcpy(initVector,State,sizeof(initVector));
				outputFile.write((const char*)State,16);
			}
			outputFile.close();
			inputFile.close();
			delete Ept;
		}
		else if(type_aes == "dec_cbc")
		{
			Decrypt* Dpt = new Decrypt();
			Dpt->InitAES((byte*)key.c_str(),keyLenght);

			std::ifstream inputFile;
			inputFile.open(inputFileName, std::ios::binary);

			byte State[16];

			std::ofstream outputFile;
			outputFile.open (outputFileName, std::ofstream::out);

			byte initVector[16];
			byte ciphertext[16];
			memset(initVector,0,sizeof(initVector));
			memset(initVector,0,sizeof(ciphertext));

			while(inputFile.peek()!=EOF)
			{
				inputFile.read((char*)State,16);
				memcpy(ciphertext,State,sizeof(ciphertext));
				Dpt->ExecuteAES(State);
				xorblock16(State,initVector);
				memcpy(initVector,ciphertext,sizeof(ciphertext));
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
			delete Dpt;
		}

		std::cout<<"The AES Execution is done, Press c to continue, other to quit\n";

		std::string inputStringTmp;
		std::cin>>inputStringTmp;
		if(inputStringTmp == "c")
		{
			continue;
		}
		else
		{
			break;
		}
	}
	return 0;
}
