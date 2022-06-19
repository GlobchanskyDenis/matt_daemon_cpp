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
	int		pid;

	if (IsLocked()) {
		logger->LogWarning("already locked");
		delete logger;
		delete sock;
		exit(-1);
	}

	if (opendir(LOG_FOLDER) == NULL)
	{
		if (mkdir(LOG_FOLDER, 0777) < 0)
		{
			std::cout << "No premission I guess" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
			

	switch (pid = fork()) {
	case -1:
		logger->LogError("cannot create system process");
		delete logger;
		delete sock;
		exit(-1);
	case 0:
		/*	Это процесс - потомок (демон)  */
		break ;
	default:
		/*	Это - процесс - родитель
		**	Ничего не закрываю так как это повлияет на потомка
		**	Вывожу в консоль pid чтобы можно было*/
		std::cout << "pid " << pid << std::endl;
		exit(0);
	}

	Lock();

	if (sock->Dial(logger) < 0) {
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

