#include "configure.h"
#include "encrypt.h"
#include "decrypt.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


#define PAGE_SIZE		4096
#define BUFF_SIZE		PAGE_SIZE*2


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
		std::string key(32,0);
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
		std::string keyString;
		std::getline(f,keyString);
		hexStrToBinaryStr((byte*)keyString.c_str(), (byte*)key.c_str(), keyString.length());
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
		TRACE("input_file: %s", inputFileName.c_str());
		TRACE("output_file: %s", outputFileName.c_str());

		//begin encrypt or Decrypt
		// no salt, no key-derivation function, padding is PKCS#7
		AES* pnt = NULL;
		if (type_aes == "enc_ecb" || type_aes == "enc_cbc")
		{
			pnt = new Encrypt();
		}
		else if (type_aes == "dec_ecb" || type_aes == "dec_cbc")
		{
			pnt = new Decrypt();
		}

		pnt->InitAES((byte*)key.c_str(),keyLenght);

		std::ifstream inputFile;
		inputFile.open(inputFileName, std::ios::binary);

		byte* State = NULL;

		std::ofstream outputFile;
		outputFile.open (outputFileName, std::ofstream::out | std::ios::binary);

		byte cipherText[16];
		memset(cipherText,0,sizeof(cipherText));
		byte initVector[16];
		memset(initVector,0,sizeof(initVector));

		byte bufferIn[BUFF_SIZE];
		byte bufferOut[BUFF_SIZE];

		int i = 0;
		int extracted = 0;
		while(true)
		{
			if (i == 0)
			{
				extracted = inputFile.read((char*)bufferIn,BUFF_SIZE).gcount();
				TRACE("extracted = %d", extracted);
			}
			State = reinterpret_cast<byte*>(bufferIn+i);

			if (type_aes == "enc_ecb")
			{
				if (inputFile.peek() == EOF && extracted < 16)	//padding is PKCS#7
				{
					byte paddingValue = 16 - extracted;
					if(extracted !=0)
					{
						memset(State + 16 - paddingValue,paddingValue,paddingValue);
					}
				}
				pnt->ExecuteAES(State);
			}
			else if(type_aes == "dec_ecb")
			{
				pnt->ExecuteAES(State);
			}
			else if (type_aes == "enc_cbc")
			{
				if (inputFile.peek() == EOF && extracted < 16)	//padding is PKCS#7
				{
					byte paddingValue = 16 - extracted;
					if(extracted !=0)
					{
						TRACE("paddingValue = %d", paddingValue);
						memset(State + 16 - paddingValue,paddingValue,paddingValue);
						TRACE("State[15] = %d",  State[15]);
					}
				}
				xorblock16(State,cipherText);
				pnt->ExecuteAES(State);
				memcpy(cipherText,State,sizeof(cipherText));
			}
			else if (type_aes == "dec_cbc")
			{
				memcpy(cipherText,State,sizeof(cipherText));
				pnt->ExecuteAES(State);
				xorblock16(State,initVector);
				memcpy(initVector,cipherText,sizeof(cipherText));
			}

			memcpy(bufferOut + i,State,16);
			TRACE("State[15] = %d",  State[15]);
			i+=16;
			if (i == BUFF_SIZE)
			{
				i = 0;
				if (inputFile.eof() && (type_aes == "enc_cbc" || type_aes == "enc_ecb"))
				{
					memset(bufferOut + i,16,16);
					outputFile.write((const char*)bufferOut, i+16);
					break;
				}
				else if (inputFile.eof() && (type_aes == "dec_cbc" || type_aes == "dec_ecb"))
				{
					outputFile.write((const char*)bufferOut, i - State[15]);
					break;
				}
				else
				{
					outputFile.write((const char*)bufferOut,BUFF_SIZE);
				}

			}
			else if (inputFile.eof() && extracted == 16 && (type_aes == "dec_cbc" || type_aes == "dec_ecb"))
			{
				outputFile.write((const char*)bufferOut, i - State[15]);
				TRACE("State[15] = %d , i = %d",  State[15], i);
				break;
			}
			else if (inputFile.eof() && extracted < 16 && (type_aes == "enc_cbc" || type_aes == "enc_ecb"))
			{
				TRACE("State[15] = %d , i = %d",  State[15], i);
				if (extracted != 0)
				{
					outputFile.write((const char*)bufferOut, i);
				}
				else
				{
					memset(bufferOut + i,16,16);
					outputFile.write((const char*)bufferOut, i+16);
				}
				break;
			}
			extracted -=16;
		}
		outputFile.close();
		inputFile.close();
		delete pnt;

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
