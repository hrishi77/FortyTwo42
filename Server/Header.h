#ifdef _WIN32
        #pragma comment(lib, "Ws2_32.lib")
        #pragma warning(disable:4996)

        #include <WinSock2.h>
        #include <Ws2tcpip.h>
        #include <signal.h>    
        
        #include "/DH-AES(windows)/cryptopp/osrng.h"
        using CryptoPP::AutoSeededRandomPool;

        #include "/DH-AES(windows)/cryptopp/cryptlib.h"
        using CryptoPP::Exception;

        #include "/DH-AES(windows)/cryptopp/hex.h"
        using CryptoPP::HexEncoder;
        using CryptoPP::HexDecoder;

        #include "/DH-AES(windows)/cryptopp/filters.h"
        using CryptoPP::StringSink;
        using CryptoPP::StringSource;
        using CryptoPP::StreamTransformationFilter;

        #include "/DH-AES(windows)/cryptopp/des.h"
        using CryptoPP::DES_EDE2;

        #include "/DH-AES(windows)/cryptopp/modes.h"
        using CryptoPP::CBC_Mode;

        #include "/DH-AES(windows)/cryptopp/secblock.h"
        using CryptoPP::SecByteBlock;

        #include "/DH-AES(windows)/cryptopp/modes.h"
        #include "/DH-AES(windows)/cryptopp/aes.h"
        #include "/DH-AES(windows)/cryptopp/filters.h"

        #include <stdexcept>
        using std::runtime_error;

        #include "/DH-AES(windows)/cryptopp/osrng.h"
        using CryptoPP::AutoSeededRandomPool;

        #include "/DH-AES(windows)/cryptopp/integer.h"
        using CryptoPP::Integer;

        #include "/DH-AES(windows)/cryptopp/nbtheory.h"
        using CryptoPP::ModularExponentiation;

        #include "/DH-AES(windows)/cryptopp/secblock.h"
        using CryptoPP::SecByteBlock;
    #else
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include <netdb.h>
        #include <sys/uio.h>
        #include <sys/time.h>
        #include <sys/wait.h>
        #include <fcntl.h>
        #include<math.h>
        #include <algorithm>
        
        #include "cryptopp/modes.h"
        #include "cryptopp/aes.h"
        #include "cryptopp/filters.h"
        
        #include <stdexcept>
        using std::runtime_error;

        #include "osrng.h"
        using CryptoPP::AutoSeededRandomPool;

        #include "integer.h"
        using CryptoPP::Integer;

        #include "nbtheory.h"
        using CryptoPP::ModularExponentiation;

        #include "dh.h"
        using CryptoPP::DH;

        #include "secblock.h"
        using CryptoPP::SecByteBlock;

        #include <hex.h>
        using CryptoPP::HexEncoder;

        #include <filters.h>
        using CryptoPP::StringSink;
#endif

#include <stdio.h>
#include <iomanip>
#include <sstream>

#include <iostream>
	using std::cout;
	using std::cerr;
	using std::endl;

#include <string>
	using std::string;

#include <fstream>
    using std::cout;
    using std::cin;
    using std::ofstream;
    using std::endl;

#include <cstdlib>
    using std::exit;



