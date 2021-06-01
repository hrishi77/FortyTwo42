
#include "base64_encode.h"


void ENC(std::string Input_File)
{
    std::string sKey;
    // Created in use it from file 
    std::fstream Key_file;                      
    Key_file.open("KEY_S");                  
    
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
//====================================================================================================
    std::string plaintext,ciphertext,temp;

    // Input file  
    std::fstream Filein;                      
    Filein.open(Input_File.c_str());                  
    
    if(Filein.is_open()) 
    {    
        while(! Filein.eof() )
        {
            (getline(Filein,temp,' ')); 
            { 
                //Filein >> plaintext ;
                plaintext += temp; 
            }
        }
        Filein.close();
    }
    //
    // Plain Text
    //
    std::cout << "Plain Text is: (" << plaintext.size() << " bytes)" << std::endl;
    std::cout << plaintext << std::endl;
    
    //
    // Create Cipher Text
    //
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ));
   
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length());
    stfEncryptor.MessageEnd();
//=================================================================================================
   std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.length());
   std::cout << "Encoded: " << std::endl << encoded << std::endl << std::endl;
//==================================================================================================
   
    std::ofstream Enc_file;
	Enc_file.open("ENC.txt", std::ios::out | std::ofstream::trunc | std::ios::binary);
	if (Enc_file.is_open())
	{
		Enc_file << encoded;
        Enc_file.close();
		std::cout << "Wrote encrypted message to ENC.txt file " << std::endl;
	}
    Enc_file.close();
}

