/* This file was created by Billy Tetrud. Use of this file is free as long as it is credited.

 #define useWinSock  if you want to use winsock to make a shared library is not neccessary
 must use mainH (stands for main handle) instead of main()

 These are the defined objects:
		
	int netInit()
	void netQuit()
	typedef struct IPADD
	{	int host;            // 32-bit IPv4 host address 
   		int port;            // 16-bit protocol port 
	} IPaddress;
	
	class TCPsoc
	{public:
		TCPsocket sock;
		
		TCPsoc()
		
		// Turns socket into a server and listens on the port
		// returns true if it failed
		bool Listen(Uint16 port)
		
		// gets the address of the peer this socket is connected to
		// returns true on failure
		bool getPeerAdd(IPaddress& ip)
		
		// Allows a server to accept a connection
		// even if connection returns non-NULL it might still not be connected
		// if that happens try testing if you can getPeerAdd
		inline bool Accept(TCPsoc& a)
		
		// connects to a server
		// returns true on error
		bool Connect(char* hostname, Uint16 port)
		
		// receives len bytes of data into container
		// blocks until len bytes is received
		// returns number of bytes received
		// if 0 is returned, the socket was closed from the opposite side
		// if 0 or len isn't returned there was an error
		inline int Recv(void* container, int len)

		// attempts to send len bytes of data
		// returns number of bytes sent
		inline int Send(void *data, int len)
		
		inline void Close()
		
		inline TCPsoc& operator =(TCPsoc& a)
		
	};


	class UDPsoc
	{public:
		UDPsocket sock;
		IPaddress dest;
	
		void UDPsoc
	
		// gets the address of the peer this socket is connected to
		// returns true on failure
		bool getPeerAdd(IPaddress& ip)
	
		// binds a UDPsoc to its destination
		// return true on failure
		bool Bind(char* hostname, int port)
		
		// binds a UDPsoc to its destination
		// void because this can't fail
		void BindRaw(IPaddress& addr)
		
		// receives len bytes of data into container
		// blocks until len bytes is received
		// returns number of bytes received
		// if 0 is returned, the socket was closed from the opposite side
		// if 0 or len isn't returned there was an error
		inline int Recv(void* container, int len, IPaddress& into)
		
		// attempts to send len bytes of data
		// returns true on failure
		inline bool Send(void* data, int len)
		
		inline void Close()
		
	};

	
*/

		/* 
	#if defined(useWinSock)
		Must use:
		
		// some windows library like wsock32.lib or winsock32.lib 
		*/
		/*
		#include <winsock.h>

		{
	    WSADATA wsaData;   // if this doesn't work
	    //WSAData wsaData; // then try this instead
	
	    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
	        fprintf(stderr, "WSAStartup failed.\n");
	        exit(1);
	    }
		//*/


#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
	/* 
	Must use:
	
-lSDLmain 
-lSDL 
-lsdl_net
	*/
	
	#include <SDL/SDL.h>
	#include <SDL/SDL_net.h>
	
	#include <windows.h>
	#define mainH int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) 

#else	// linux and unix

	#include <sys/socket.h>
	#include <netdb.h>
	#include <net/if.h>
	
	#define mainH main(int argc, char** argv)
#endif
 		
	// initialize - returns 0 on success
	// returns 1 on SDL_Init failure
	// returns 2 on SDLNet_Init failure
	int netInit()
	{	
		#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
			if(!SDL_Init(0))
			{	if(SDLNet_Init()<0)
				{	return 2;
				}
			}else 
			{	return 1; 
			}
			return 0;
		#else	// nothing for linux
			return 0;
		#endif
	}
	
	// quits SDL and SDL_net
	void netQuit()
	{	
		#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
			SDLNet_Quit(); 
			SDL_Quit();
		#else	// nothing for linux
		#endif
	}
	
	#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
		// already defined in SDL_net
	#else
		typedef struct IPADD
		{	int host;            // 32-bit IPv4 host address 
   			int port;            // 16-bit protocol port 
		} IPaddress;
	#endif
	
	// puts the first IP address associated with hostname into IP
	void Resolve(IPaddress& IP, char* hostname)
	{	
		#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
			SDLNet_ResolveHost(&IP,hostname,0);
		#else
			// get IPaddress of hostname
			struct hostent* host = gethostbyname(hostname);		
			// set IPaddress in structure
			IP.host = ((struct in_addr*)(host->h_addr_list)[0])->s_addr;	
		#endif
	}
	
	
	class TCPsoc
	{public:
		#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
			TCPsocket sock;
		#else
			int sock;
		#endif
		
		TCPsoc()
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				// nothing for SDL_net
			#else
				sock = socket(PF_INET, SOCK_STREAM, 0);
			#endif
		}
		
		// Turns socket into a server and listens on the port
		// returns true if it failed
		bool Listen(Uint16 port)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				IPaddress portHolder;
				TCPsocket portHolderSocket;
				
				portHolder.port = port;
				portHolderSocket = SDLNet_TCP_Open(&portHolder); 		// open the tcp server socket 
				if(!portHolderSocket)
				{	sock=0;
					return true;	// fail.
				} 
				sock = portHolderSocket;
				return false;	// no fail
			#else
				struct sockaddr_in my_addr;

				my_addr.sin_family = AF_INET;         // host byte order
				my_addr.sin_port = htons(port);     // short, network byte order
				my_addr.sin_addr.s_addr = htons(INADDR_ANY);
				memset(&(my_addr.sin_zero), '\0', sizeof(my_addr.sin_zero));
		
				if(-1 == bind(sock, (struct sockaddr *)&my_addr, sizeof(my_addr)))
					return true;
				//else
					return false;
				
				if(-1 == listen(sock, 128))		// set "backlog" to 128
				{	return true;	// fail
				}
				return false;
			#endif
		}	
		
		// gets the address of the peer this socket is connected to
		// returns true on failure
		bool getPeerAdd(IPaddress& ip)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				IPaddress* temp;
				temp = SDLNet_TCP_GetPeerAddress(sock);	// not sure what kind of memory this returns
				if(temp==NULL)
					return true;
				ip.host = temp->host;
				ip.port = temp->port;
				return false;
			#else
				struct sockaddr_in addr;
				
				int len = sizeof(addr);
				if(getpeername(sock, (struct sockaddr*)&addr, &len))
					return true;
				
				ip.host = addr.sin_addr.s_addr;
				ip.port = addr.sin_port;
				return false;
			#endif
		}
		
		
		// Allows a server to accept a connection
		// even if connection returns non-NULL it might still not be connected
		// if that happens try testing if you can getPeerAdd
		inline bool Accept(TCPsoc& a)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				a.sock = SDLNet_TCP_Accept(sock);		//try to accept a connection
				if(a.sock == NULL)
					return false;
				else
					return true;
			#else
				struct sockaddr_in cliaddr;
				socklen_t *addrlen;
				
				int len = sizeof(cliaddr);
				if(-1==(a.sock = accept(sock, (struct sockaddr *)&cliaddr, &len)))
				{	//printf("listener->sock: %d\n",listener->sock);
					return false;	// no accept
				}
				
				if (a.sock)
				{	IPaddress iptemp;
					
					if(a.getPeerAdd(iptemp))		// get the clients IP and port number        
					{	return true;	// accept
					}
					else
					{	return false;	// no accept
					}
				}
			#endif
		}
		
		// connects to a server
		// returns true on error
		bool Connect(char* hostname, Uint16 port)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				IPaddress ip;
				if(SDLNet_ResolveHost(&ip,hostname,port)<0)	// Resolve the argument into an IPaddress type 
				{	sock=0;
				}
				sock=SDLNet_TCP_Open(&ip);  // open socket 
				if(sock==NULL)
					return true;
				//else
				return false;
			#else
				struct sockaddr_in serverAdd;   // will hold the destination addr

				serverAdd.sin_family = AF_INET;          // host byte order
			    serverAdd.sin_port = htons(port);   // short, network byte order
			    struct hostent* host = gethostbyname(hostname);		// get IPaddress of hostname
				serverAdd.sin_addr.s_addr = ((struct in_addr*)(host->h_addr_list)[0])->s_addr;	// set IPaddress in structure
			    memset(&(serverAdd.sin_zero), 0, sizeof(serverAdd.sin_zero));	// set last part of struct to 0 (might be optional?)
		
				if(connect(sock, (struct sockaddr *)&serverAdd, sizeof(serverAdd)))
					return true; // for error
				//else
				return false;
			#endif
		}
		
		// receives len bytes of data into container
		// blocks until len bytes is received
		// returns number of bytes received
		// if 0 is returned, the socket was closed from the opposite side
		// if 0 or len isn't returned there was an error
		inline int Recv(void* container, int len)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				return SDLNet_TCP_Recv(sock, container, len);
			#else
				return recv(sock, container, len, 0); 
			#endif
		}

		// attempts to send len bytes of data
		// returns number of bytes sent
		inline int Send(void *data, int len)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				return SDLNet_TCP_Send(sock, data, len);
			#else
				return send(sock, data, len, 0); 
			#endif
		}
		
		inline void Close()
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				SDLNet_TCP_Close(sock);
			#else
				close(sock);
			#endif
		}
		
		inline TCPsoc& operator =(TCPsoc& a)
		{	sock = a.sock;
		}
		
	};


	class UDPsoc
	{public:
		
		#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
			UDPsocket sock;
			IPaddress dest;
		#else
			int sock;
			struct sockaddr_in dest;
		#endif
	
		UDPsoc()
		{
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				// nothing needs to happen
			#else
				sock = socket(PF_INET, SOCK_DGRAM, 0);
				dest.sin_family = AF_INET;				// host byte order
				memset(&(a->dest.sin_zero), 0, sizeof(a->dest.sin_zero));	// set last part of struct to 0 (might be optional?)
			#endif
		}
	
		// gets the address of the peer this socket is connected to
		// returns true on failure
		bool getPeerAdd(IPaddress& ip)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				IPaddress* temp;
				temp = SDLNet_UDP_GetPeerAddress(sock, -1);	// not sure what kind of memory this returns
				if(temp==NULL)
					return true;
				ip.host = temp->host;
				ip.port = temp->port;
				return false;
			#else
				struct sockaddr_in addr;
				
				int len = sizeof(addr);
				if(getpeername(sock, (struct sockaddr*)&addr, &len))
					return true;
				
				ip.host = addr.sin_addr.s_addr;
				ip.port = addr.sin_port;
				return false;
			#endif
		}
	
		// Opens on a UDP port
		// Opening on port 0 will open an unused port
		// returns true on failure
		bool Open(int port)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				sock = SDLNet_UDP_Open(port);
				if(sock==NULL)
					return true;
			#else
				struct sockaddr_in my_addr;
		
				my_addr.sin_family = AF_INET;         // host byte order
				my_addr.sin_port = htons(port);     // short, network byte order
				my_addr.sin_addr.s_addr = htons(INADDR_ANY);
				memset(&(my_addr.sin_zero), '\0', sizeof(my_addr.sin_zero));
		
				if(-1 == bind(a->sock, (struct sockaddr *)&my_addr, sizeof(my_addr)))
					return true;
			#endif
			
			return false;
		}
	
		// binds a UDPsoc to its destination
		// void because this can't fail
		void Bind(char* hostname, int port)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				IPaddress ip;
				Resolve(ip,hostname);
				dest.host = ip.host;
				dest.port = port;
			#else
				dest.sin_port = htons(port);   // short, network byte order
				struct hostent* host = gethostbyname(hostname);		// get IPaddress of hostname
				dest.sin_addr.s_addr = ((struct in_addr*)(host->h_addr_list)[0])->s_addr;	// set IPaddress in structure
			#endif
		}
		
		// binds a UDPsoc to its destination
		// void because this can't fail
		bool BindRaw(IPaddress& addr)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				dest.host = addr.host;
				dest.port = addr.port;
			#else
				dest.sin_port = addr.port;
				dest.sin_addr.s_addr = addr.host;
			#endif
		}
		
		// receives len bytes of data into container
		// blocks until len bytes is received
		// returns number of bytes received, 0 on error
		inline int Recv(void* container, int len, IPaddress& into)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				UDPpacket packet;
				packet.data = (Uint8*)container;
				int ready=0;
				while(ready==0)
				{	ready=SDLNet_UDP_Recv(sock, &packet);
					SDL_Delay(10);
				}
				if(ready==-1)
					return 0;
				into.host = packet.address.host;
				into.port = packet.address.port;
				
				return packet.len;
			#else
				struct sockaddr_in addr;
				
				int fromLen = sizeof(addr);
				int bytes;
				bytes = recvfrom(sock, container, len, 0, (struct sockaddr*)&addr, &fromLen); 
				
				into.host = addr.sin_addr.s_addr;
				into.port = addr.sin_port; 
				
				return bytes;
			#endif
		}	
		
		// receives len bytes of data into container
		// blocks until len bytes is received
		// returns number of bytes received
		// if 0 is returned, the socket was closed from the opposite side
		// if 0 or len isn't returned there was an error
		inline int Recv(void* container, int len)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				UDPpacket packet;
				packet.data = (Uint8*)container;
				SDLNet_UDP_Recv(sock, &packet);
				return packet.len;
			#else
				struct sockaddr_in addr;
				
				int fromLen = sizeof(addr);
				int bytes;
				bytes = recvfrom(sock, container, len, 0, (struct sockaddr*)&addr, &fromLen); 
				
				return bytes;
			#endif
		}	
		
		// attempts to send len bytes of data
		// returns true on failure
		inline bool Send(const void* data, int len)
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				UDPpacket packet;
				packet.data = (Uint8*)data;
				packet.len = len;
				packet.maxlen = len;
				packet.address.host = dest.host;
				packet.address.port = dest.port;
				
				if(SDLNet_UDP_Send(sock, -1, &packet) == 0)
				{	return true;
				}
			#else
				if(sendto(sock, data, len, 0, (struct sockaddr*)&(dest), sizeof(dest)) == -1)
					return true;
			#endif
			return false; // no error
		}
		
		inline void Close()
		{	
			#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
				SDLNet_UDP_Close(sock);
			#else
				close(sock);
			#endif
		}
		
	};
