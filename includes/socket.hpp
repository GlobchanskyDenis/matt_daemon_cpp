#ifndef SOCKET
# define SOCKET

# include <iostream>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <netinet/in.h>


# include "config.hpp"
# include "reporter.hpp"
# include "utils.hpp"

class Socket
{
	private:
		int		server_fd;
		int 	new_socket;
		struct sockaddr_in address;
		int 	addrlen;
		int writeTCP(const char *);
		int writeUDP(const char *);
		char* readTCP(void);
		char* readUDP(void);
	public:
		int  Dial(Reporter*); // 0 - OK, -1 - FAIL
		void Close(void);
		char* Read(void);
		int Write(const char *); // 0 - OK, -1 - FAIL
	
		/*	Конструктор  */
		Socket() {
			this->server_fd = 0;
			this->new_socket = 0;
		}

		/*	Деструктор  */
		~Socket()
		{
			this->Close();
		};
};

#endif