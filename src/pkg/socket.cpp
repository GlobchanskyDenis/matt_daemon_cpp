#include "socket.hpp"

int     Socket::Dial(void) {
	// int server_fd;
    // struct sockaddr_in address;
    int opt = 1;
    int connType;

    /*  В зависимости от того что мы указали в конфигурационном config.hpp выбираем тип подключения  */
    if (IS_TCP_SOCKET) {
        connType = SOCK_STREAM;
    } else {
        connType = SOCK_DGRAM;
    }

    /*  Получаем дескриптор сокета. AF_INET это тип домена - IPv4, 0 - это протокол (не смог найти описание что бы это значило)  */
    if ((this->server_fd = socket(AF_INET, connType, 0))
        == 0) {
        std::cout << "socket failed" << std::endl;
        return (-1);
    }

    /*  Задаем опции  */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "setsockopt" << std::endl;
        return (-1);
    }
    this->address.sin_family = AF_INET;

    /*  Заполняю Ip и порт в специальную структуру (IP, PORT содержатся в config.hpp)  */
    if (inet_aton(IP, &(this->address.sin_addr)) == 0) {
        std::cout << "ip адрес " << IP << " некорректный. Проверьте config.hpp" << std::endl;
        return (-1);
    }
    this->address.sin_port = htons(PORT);

    this->addrlen = sizeof(this->address);

    /*  Бинд - это привязка созданного сокета к ip адресу и порту  */
    if (bind(server_fd, (struct sockaddr*)&(this->address), this->addrlen) < 0) {
        std::cout << "bind failed" << std::endl;
        return (-1);
    }

    if (IS_TCP_SOCKET) {
         /*  listen означает "пассивный" режим, то есть - сервер  */
        if (listen(server_fd, 3) < 0) {
            std::cout << "listen failed" << std::endl;
            return (-1);
        }

        /*  accept извлекает первый запрос на соединение из очереди ожидающих соединений для слушающего сокета,
        **  sockfd, создает новый подключенный сокет и возвращает новый файловый дескриптор, ссылающийся на этот
        **  сокет. На этом этапе между клиентом и сервером устанавливается соединение, и они готовы к передаче данных  */
        if ((this->new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&(this->addrlen))) < 0) {
            std::cout << "accept failed" << std::endl;
            return (-1);
        }
    }
   
    return 0;
}

int   Socket::Write(const char* message) {
    if (IS_TCP_SOCKET) {
        return this->writeTCP(message);
    } else {
        return this->writeUDP(message);
    }
}

int   Socket::writeTCP(const char* message) {
    if (send(this->new_socket, message, strlen(message), 0) <= 0) {
        return (-1);
    }
    return 0;
}

int   Socket::writeUDP(const char* message) {
    if (sendto(this->server_fd, message, strlen(message), MSG_CONFIRM, (struct sockaddr *)&(this->address), this->addrlen) <= 0) {
        return (-1);
    }
    return 0;
}

char*   Socket::Read(void) {
    if (IS_TCP_SOCKET) {
        return this->readTCP();
    } else {
        return this->readUDP();
    }
}

char*   Socket::readTCP(void) {
    char *buffer = strnew(MAX_SOCKET_MESSAGE_LENGTH);
    if (buffer == NULL) {
        return NULL;
    }
    if (read(this->new_socket, buffer, MAX_SOCKET_MESSAGE_LENGTH) <= 0) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

char*   Socket::readUDP(void) {
    char *buffer = strnew(MAX_SOCKET_MESSAGE_LENGTH);
    if (buffer == NULL) {
        return NULL;
    }
    if (recvfrom(this->server_fd, buffer, MAX_SOCKET_MESSAGE_LENGTH, MSG_WAITALL, (struct sockaddr *)(&(this->address)), (socklen_t*)&(this->addrlen)) <= 0) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

void	Socket::Close(void) {
	if (this->new_socket != 0) {
		close(this->new_socket);
		this->new_socket = 0;
        this->server_fd = 0;
	} else if (this->server_fd != 0) {
        close(this->server_fd);
		this->new_socket = 0;
        this->server_fd = 0;
    }
}
