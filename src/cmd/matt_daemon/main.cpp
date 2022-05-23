#include "matt_daemon.hpp"

// using namespace std;

int	logLoop(Socket* sock, Tintin_reporter* logger) {
	char* line;
	while (true) {
		if ((line = sock->Read()) == NULL) {
			return (-1);
		}
		trim(line, MAX_SOCKET_MESSAGE_LENGTH);
		if (isEqual(line, "exit", MAX_SOCKET_MESSAGE_LENGTH) == true) {
			free(line);
			break ;
		}
		logger->Log(line);
		free(line);
	}
	return 0;
}

int main(void)
{
	Tintin_reporter* logger = new Tintin_reporter();
	Socket* sock = new Socket();
	// int		pid;

	if (IsLocked()) {
		logger->LogWarning("already locked");
		delete logger;
		delete sock;
		exit(-1);
	}

	// switch (pid = fork()) {
	// case -1:
	// 	logger->LogError("cannot create system process");
	// 	delete logger;
	// 	delete sock;
	// 	exit(-1);
	// case 0:
	// 	/*	Это процесс - потомок  */
	// 	// std::cout << "pid " << pid << std::endl;
	// 	stdin = fopen("/dev/null", "r");
	// 	stdout = fopen("/dev/null", "w+");
	// 	stderr = fopen("/dev/null", "w+");
	// 	break ;
	// default:
	// 	/*	Это - процесс - родитель  */
	// 	std::cout << "pid " << pid << std::endl;
	// 	exit(0);
	// }

	Lock();

	if (sock->Dial() < 0) {
		logger->LogError("socket dial failed");
		Unlock();
		logger->LogInfo("lock file was removed");
		delete logger;
		delete sock;
		exit(-1);
	}

	if (auth(sock, logger) < 0) {
		logger->LogError("auth fail");
		Unlock();
		logger->LogInfo("lock file was removed");
		delete logger;
		delete sock;
		exit(-1);
	}
	if (logLoop(sock, logger) < 0) {
		logger->LogError("fail reading socket");
		Unlock();
		logger->LogInfo("lock file was removed");
		delete logger;
		delete sock;
		exit(-1);
	}

	Unlock();
	logger->LogInfo("lock file was removed");
	delete logger;
	delete sock;
}

