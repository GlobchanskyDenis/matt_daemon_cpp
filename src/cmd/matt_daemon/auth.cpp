#include "config.hpp"
#include "socket.hpp"
#include "reporter.hpp"
#include "utils.hpp"

char*	readLogin(Socket* sock) {
	char* login;
	if (sock->Write("Введите логин\n") < 0) {
		return NULL;
	}
	if ((login = sock->Read()) == NULL) {
		return NULL;
	}
	trim(login, MAX_SOCKET_MESSAGE_LENGTH);
	/*	В случае UDP сокета мы можем перехватить эхо нашего предыдущего сообщения которое мы же и отправили, если это так прочитаем сокет еще раз  */
	if (IS_TCP_SOCKET == false && isEqual(login, "Введите логин", MAX_SOCKET_MESSAGE_LENGTH) == true) {
		if ((login = sock->Read()) == NULL) {
			return NULL;
		}
		trim(login, MAX_SOCKET_MESSAGE_LENGTH);
	}
	return login;
}

char*	readPassword(Socket* sock) {
	char* password;
	if (sock->Write("Введите пароль\n") < 0) {
		return NULL;
	}
	if ((password = sock->Read()) == NULL) {
		return NULL;
	}
	trim(password, MAX_SOCKET_MESSAGE_LENGTH);
	/*	В случае UDP сокета мы можем перехватить эхо нашего предыдущего сообщения которое мы же и отправили, если это так прочитаем сокет еще раз  */
	if (isEqual(password, "Введите пароль", MAX_SOCKET_MESSAGE_LENGTH) == true) {
		if ((password = sock->Read()) == NULL) {
			return NULL;
		}
		trim(password, MAX_SOCKET_MESSAGE_LENGTH);
	}
	return password;
}

int	auth(Socket* sock, Tintin_reporter* logger) {
	char* login;
	char* password;
	const char* password_hash;
	while (true) {
		if ((login = readLogin(sock)) == NULL) {
			return (-1);
		}
		if ((password = readPassword(sock)) == NULL) {
			free(login);
			return (-1);
		}
		if ((password_hash = hash_this(password)) == NULL) {
			free(login);
			free(password);
			return (-1);
		}
		if ((isEqual(login, SERVER_LOGIN, MAX_SOCKET_MESSAGE_LENGTH) == true) && (isEqual(password_hash, SERVER_PASSWORD_HASH, MAX_SOCKET_MESSAGE_LENGTH) == true)) {
			free(login);
			free(password);
			free((char *)password_hash);
			break ;
		}
		free(login);
		free(password);
		free((char *)password_hash);
	}
	logger->LogInfo("User authenticated successfully");
	if (sock->Write("Авторизация успешна\n") < 0) {
		free(login);
		return (-1);
	}
	return 0;
}

