//Server   
#include "Header.h"
#include "Encrypt.h"
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

//send File
 #ifdef _WIN32
        void sentFile(SOCKET sockfd, char* Input_File)
        {
            // for read operation from file and used to sent operation
            char buff[MAX];       

            // create file
            FILE* fp;
            fp = fopen(Input_File, "r");  

            if (fp == NULL)
            {
                std::cout << "Error IN Opening File .. " << std::endl;
                return;
            }

            // fgets reads upto MAX character or EOF
            while (fgets(buff, MAX, fp) != NULL)    
                // sent the file data to stream
                send(sockfd, buff, sizeof(buff), 0);
            
            // close the file
            fclose(fp);       
            std::cout << "File Sent successfully " << std::endl;
        }
 #else
        void sentFile(int sockfd, char* Input_File)
        {
            // for read operation from file and used to sent operation
            char buff[MAX];       

            // create file
            FILE* fp;
            fp = fopen(Input_File, "r");  

            if (fp == NULL)
            {
                std::cout << "Error IN Opening File .. " << std::endl;
                return;
            }
            
            // fgets reads upto MAX character or EOF
            while (fgets(buff, MAX, fp) != NULL)    
            // sent the file data to stream
            write(sockfd, buff, sizeof(buff));        
            
            // close the file
            fclose(fp);       
            std::cout << "File Sent successfully " << std::endl;
        }
#endif


//Server side
int main(int argc, char* argv[])
{
    #ifdef _WIN32
            u_short port = 2121;
            
            //msg to send and receive messages 
            char buffer[1024];

            //SOCKETS
            WSADATA WSAData;
            SOCKET server, client;
            SOCKADDR_IN serverAddr;

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            //serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(port);

            WSAStartup(MAKEWORD(2, 0), &WSAData);
            server = socket(AF_INET, SOCK_STREAM, 0);
           
            if (server < 0)
            {
                std::cerr << "Error establishing the server socket" << std::endl;
                exit(0);
            }

            //bind the socket to its local address
            int bindStatus = bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
            if (bindStatus < 0)
            {
                std::cerr << "Error binding socket to local address" << std::endl;
                exit(0);
            }
            cout << "Listening for incoming connections..." << endl;
            //listen for up to 5 requests at a time
            listen(server, 5);

            //receive a request from client using accept
            //we need a new address to connect with the client
            SOCKADDR_IN clientAddr;
            int clientAddrSize = sizeof(clientAddr);

            if ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET)
            {
                std::cerr << "Error accepting request from client!" << std::endl;
                exit(1);
            }
            cout << "Client connected!" << endl;

    #else
            //for the server, we only need to specify a port number
             if (argc != 2)
             {
                 std::cerr << "Usage: port" << std::endl;
                 exit(0);
             }

             //grab the port number
             int port = atoi(argv[1]);

             //msg to send and receive messages
             char msg[1024];

             //setup a socket and connection tools
             sockaddr_in servAddr;
             //clear the complete structure usnig bzero
             bzero((char*)&servAddr, sizeof(servAddr));

             servAddr.sin_family = AF_INET;                          //IPv4 Internet concept IP
             servAddr.sin_addr.s_addr = htonl(INADDR_ANY);           //get your own addrss ie.PC
             servAddr.sin_port = htons(port);                        //int no to network format
             
             //open stream oriented socket with internet address also keep track of the socket descriptor
             int serverSd = socket(AF_INET, SOCK_STREAM, 0);
             int option = 1;
             setsockopt(serverSd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

             if (serverSd < 0)
             {
                 std::cerr << "Error establishing the server socket" << std::endl;
                 exit(0);
             }

             //bind the socket to its local address
             int bindStatus = bind(serverSd, (struct sockaddr*) & servAddr, sizeof(servAddr));
             if (bindStatus < 0)
             {
                 std::cerr << "Error binding socket to local address" << std::endl;
                 exit(0);
}
             std::cout << "Waiting for a client to connect..." << std::endl;
             //listen for up to 5 requests at a time
             listen(serverSd, 5);

             //receive a request from client using accept
            //we need a new address to connect with the client
             sockaddr_in newSockAddr;
             socklen_t newSockAddrSize = sizeof(newSockAddr);

             //accept, create a new socket descriptor to
            //handle the new connection with client
             int newSd = accept(serverSd, (sockaddr*)&newSockAddr, &newSockAddrSize);
             if (newSd < 0)
             {
                 std::cerr << "Error accepting request from client!" << std::endl;
                 exit(1);
             }
             std::cout << "Connected with client!" << std::endl;
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

            // b's public key
            Integer y;            
            Integer kb;

            //
            //random number
            //
            srand(time(0));
            const unsigned long long int bb = (rand() % 1000000000000); //secret integer
            Integer b = bb;
            std::cout << "b :" << b << std::endl;

//-------------------------------------------------------------------------------------------------------------------------
 
            y = Power(G, b, P); //y = G^b mod P

            std::cout << "Key genration of Y = " << y << std::endl;
    
            //int to string
            std::stringstream sss;
            sss << y;
            std::string s;
            sss >> s;

            std::string data = s;
//------------------------------------------------------rec-----------------------------------------------------------------

    #ifdef _WIN32
            //receive a message from the client (listen)
            std::cout << "Awaiting client response..." << std::endl;
            recv(client, buffer, sizeof(buffer), 0);
            cout << "Client : " << buffer << endl;
            std::string ss = buffer;    
            memset(&buffer, 0, sizeof(buffer));//clear the msg
            strcpy(buffer, data.c_str());
    #else
            //receive a message from the client (listen)
            std::cout << "Awaiting client response..." << std::endl;
            memset(&msg, 0, sizeof(msg));//clear the msg
            recv(newSd, (char*)&msg, sizeof(msg), 0);
            std::cout << "Client:" << msg << std::endl;
            std::string ss = msg;
            memset(&msg, 0, sizeof(msg));//clear the msg
            strcpy(msg, data.c_str());
    #endif

//----------------------------------------------------send------------------------------------------------------------------

    #ifdef _WIN32
            //send the message to client
            send(client, buffer, sizeof(buffer), 0);
    #else
            //send the message to client
            send(newSd, (char*)&msg, strlen(msg), 0);
    #endif

//--------------------------------Secret Key genration----------------------------------------------------------------------

            Integer x;

            //string to int
            std::stringstream convert;
            convert << ss;
            convert >> x;

            kb = Power(x, b, P);
   
            if (kb != 0)
            {
                std::cout << "Secret key of Server = " << kb << std::endl;
                //write key into File
                std::fstream outfile;
                outfile.open("KEY_S", std::ios::out);
                if (outfile.is_open())
                {
                    outfile << kb;
                    outfile.close();
                }
            }

//==========================================================================================================================
    
            char Input_File[16]; //create filename variables
            std::cout << "\nEnter the input filename :\n"; //user enters file names for IO
            std::cin >> Input_File;

            ENC(Input_File);
            char ENC_File[20] = "ENC.txt";

    #ifdef _WIN32
            sentFile(client, ENC_File);
    #else   
            sentFile(newSd, ENC_File);
    #endif

//==========================================================================================================================
            //we need to close the socket descriptors after we're all done
    #ifdef _WIN32
            closesocket(client);
            WSACleanup();
    #else
            close(newSd);
            close(serverSd);
    #endif

            std::cout << "Connection closed ." << std::endl;
            return 0;
}