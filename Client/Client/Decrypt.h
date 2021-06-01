#include "base64_decode.h"

void DEC(std::string Output_File)
{
    std::string sKey;
    // Created in use it from file 
    std::fstream Key_file;
    Key_file.open("KEY_C");   
    
    if(Key_file.is_open()) 
    { 
        while (getline(Key_file,sKey)) 
        { 
            Key_file >> sKey;
        }
        Key_file.close();
    }
    
    if(CryptoPP::AES::DEFAULT_KEYLENGTH < sKey.size())
        sKey = sKey.substr(0, CryptoPP::AES::DEFAULT_KEYLENGTH); // chop if too long
    else if(CryptoPP::AES::DEFAULT_KEYLENGTH > sKey.size())
        sKey += std::string(CryptoPP::AES::DEFAULT_KEYLENGTH - sKey.size(), '*'); // pad
 
    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
    memcpy(key, sKey.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
    byte iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    std::cout << "Key enc : "<< key << std::endl;         //Key 
    std::cout << "iv enc : "<< iv << std::endl;
//===================================================================================

    std::string decryptedtext,cipher,ciphertext,temp;
    
    // Input file  
    std::fstream FileOut;                      
    FileOut.open(Output_File.c_str());                  
    
    if(FileOut.is_open()) 
    { 
        while (!FileOut.eof()) 
        {
            getline(FileOut,temp,' '); 
            { 
                //FileOut >> cipher;
                cipher += temp;
            }
        }
        FileOut.close();
    }
    std::cout << "Encoded : " << std::endl << cipher<< std::endl;

//====================================================================================================
    //decode 
        ciphertext = base64_decode(cipher);
        std::cout << "Decoded : " << std::endl << ciphertext<< std::endl<< std::endl;
//====================================================================================================
    //
    // Decrypt
    //
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ));
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size());
    stfDecryptor.MessageEnd();
    
    //
    // Decrypted Text
    //
    std::cout << "Decrypted Text of client: ";
    std::cout << decryptedtext;
    std::cout << std::endl;

    
    std::ofstream Dec_file;
	Dec_file.open("DEC.txt", std::ios::out | std::ofstream::trunc );
	if (Dec_file.is_open())
	{
		Dec_file << decryptedtext;
        Dec_file.close();
		std::cout << "Wrote encrypted message to Enc_File. " << std::endl;
	}
}
