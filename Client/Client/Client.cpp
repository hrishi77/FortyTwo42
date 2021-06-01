//Client
#include "Header.h"
#include "Decrypt.h"
#define MAX 100


//Power function y = a^m mod n
Integer Power(Integer a, Integer m, Integer n)
{
    Integer r;
    Integer y = 1;

    while (m > 0)
    {
        r = m % 2;
        if (r == 1)
            y = (y * a) % n;
        a = a * a % n;
        m = m / 2;
    }
    return y;
}

//Received file
#ifdef _WIN32 
        void recvFile(SOCKET sockfd, char* Output_File)
        {
            // to store message from client
            char buff[MAX];  
        
            FILE* fp;
            // stores the file content in Output_File in the program directory
            fp = fopen(Output_File, "w"); 

             if (fp == NULL)
             {
                 std::cout << "Error IN Opening File " << std::endl;
                 return;
             }

              while(recv(sockfd, buff, MAX, 0))
                  fprintf(fp, "%s", buff);
          
              fclose(fp);
              std::cout << "Encrypted File received successfully" << std::endl;
        }
#else
         void recvFile(int sockfd, char* Output_File)
         {
             // to store message from client
             char buff[MAX];  
         
             FILE* fp;
             // stores the file content in Output_File in the program directory       
             fp = fopen(Output_File, "w"); 

             if (fp == NULL)
             {
                 std::cout << "Error IN Opening File " << std::endl;
                 return;
             }

             while (read(sockfd, buff, MAX) > 0)
                 fprintf(fp, "%s", buff);
         
             fclose(fp);
             std::cout << "Encrypted File received successfully" << std::endl;
         }
#endif


//Client side
int main(int argc, char* argv[])
{
    #ifdef _WIN32
            u_short port = 2626;

            //create a message buffer
            char buffer[1024];

            //Declare varaible for socket and connection tool
            WSADATA WSAData;
            SOCKET server;
            SOCKADDR_IN addr;

            //IPv4 Internet concept IP
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr("192.168.221.129");
            addr.sin_port = htons(port);
     
            //try to connect...
            WSAStartup(MAKEWORD(2, 0), &WSAData);
            server = socket(AF_INET, SOCK_STREAM, 0);
            int status = connect(server, (SOCKADDR*)&addr, sizeof(addr));

            if (status < 0)
            {
                std::cout << "Error connecting to socket!" << std::endl;
                exit(1);
            }
            cout << "Connected to server!" << endl;
    #else
            //we need 2 things: ip address and port number, in that order
            if (argc != 3)
            {
                std::cerr << "Usage: ip_address port" << std::endl;
                exit(0);
            }
        
            //grab the IP address and port number 
            char* serverIp = argv[1];
            //char to int
            int port = atoi(argv[2]);

            //create a message buffer 
            char msg[1024];

            //setup a socket and connection tools 
            struct hostent* host = gethostbyname(serverIp);
            sockaddr_in sendSockAddr;
            //clear the complete structure usnig bzero
            bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 

            //IPv4 Internet concept IP
            sendSockAddr.sin_family = AF_INET;                  
            sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*) * host->h_addr_list));
            sendSockAddr.sin_port = htons(port);

            //try to connect...
            int clientSd = socket(AF_INET, SOCK_STREAM, 0);     
            int status = connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));

            if (status < 0)
            {
                std::cout << "Error connecting to socket!" << std::endl;
                exit(1);
            }
            std::cout << "Connected to the server!" << std::endl;
    #endif
     
//==========================================================================================================================
//                                  diffie hellman key exchange
//==========================================================================================================================

            // P is prime number 
            Integer P("0xB10B8F96A080E01DDE92DE5EAE5D54EC52C99FBCFB06A3C6"
                "9A6A9DCA52D23B616073E28675A23D189838EF1E2EE652C0"
                "13ECB4AEA906112324975C3CD49B83BFACCBDD7D90C4BD70"
                "98488E9C219A73724EFFD6FAE5644738FAA31A4FF55BCCC0"
                "A151AF5F0DC8B4BD45BF37DF365C1A65E68CFDA76D4DA708"
                "DF1FB2BC2E4A4371");

            // G is a primitive root modulo P.
            Integer G("0xA4D1CBD5C3FD34126765A442EFB99905F8104DD258AC507F"
                "D6406CFF14266D31266FEA1E5C41564B777E690F5504F213"
                "160217B4B01B886A5E91547F9E2749F4D7FBD7D3B9A92EE1"
                "909D0D2263F80A76A6A24C087A091F531DBF0A0169B6A28A"
                "D662A4D18E73AFA32D779D5918D08BC8858F4DCEF97C2A24"
                "855E6EEB22B3B2E5");

            Integer x;            
            Integer ka;

            //
            //random number         
            //
            srand(time_t(0));
            //const unsigned long long int aa = (rand() % 100000000000 );
            const unsigned long long int aa = int(time(NULL));
            srand(aa);
            Integer a = aa;
            std::cout << "a :" << a << std::endl;

//-------------------------------------------------------------------------------------------------------------------------
 
            x = Power(G, a, P);
            std::cout << "Key genration of x = " << x << std::endl;


            //int to std::string
            std::stringstream sss;
            sss << x;
            std::string s;
            sss >> s; 

            std::string data = s;

//----------------------------------------------------send------------------------------------------------------------------

    #ifdef _WIN32
            //clear the buffer
            memset(&buffer, 0 , sizeof(buffer));
            strcpy(buffer, data.c_str());
            //send the message to server first
            send(server, buffer, sizeof(buffer), 0);
            cout << "Message sent!" << endl;
    #else
           //clear the buffer
           memset(&msg, 0, sizeof(msg));
           strcpy(msg, data.c_str());
           //send the message to server first
           send(clientSd, (char*)&msg, strlen(msg), 0);
    #endif
    
//------------------------------------------------------rec-----------------------------------------------------------------

    #ifdef _WIN32
            std::cout << "Awaiting server response..." << std::endl;
            memset(&buffer, 0 , sizeof(buffer));
            recv(server, buffer, sizeof(buffer), 0);
            std::cout << "Server: " << buffer << std::endl;
            std::string ss = buffer;
    #else
             std::cout << "Awaiting server response..." << std::endl;
             memset(&msg, 0, sizeof(msg));//clear the buffer
             recv(clientSd, (char*)&msg, sizeof(msg), 0);
             std::cout << "Server: " << msg << std::endl;
             std::string ss = msg;
    #endif
    
//--------------------------------Secret Key genration----------------------------------------------------------------------
            Integer y;
            //string to int
            std::stringstream convert;
            convert << ss;
            convert >> y;

            ka = Power(y, a, P);
   
            if (ka != 0)
            {
                std::cout << "Secret key of Client = " << ka << std::endl;
        
                //write key into file 
                std::fstream outfile1;
                outfile1.open("KEY_C", std::ios::out);
                if (outfile1.is_open())
                {
                    outfile1 << ka;
                    outfile1.close();
                }
            }
     
//==========================================================================================================================
   
            char Output_File[16]; //create filename variables
            std::cout << "\nEnter the output filename :\n";
            std::cin >> Output_File;

    #ifdef _WIN32
            recvFile(server, Output_File);
            DEC(Output_File);
   #else
            recvFile(clientSd, Output_File);
            DEC(Output_File);
   #endif
  
            std::cout << Output_File << " Decrypted successfully into DEC.txt" << std::endl;
    
//==========================================================================================================================
 
    #ifdef _WIN32
            closesocket(server);
            WSACleanup();
            cout << "Socket closed." << endl << endl;
    #else
            close(clientSd);
            std::cout << "Connection closed ." << std::endl;
    #endif
  
    return 0;
}














