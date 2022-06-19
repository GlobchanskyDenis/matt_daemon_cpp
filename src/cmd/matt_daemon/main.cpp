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
	int				pid;

	/*	Проверка прав данных программе  */
	if (opendir(LOG_FOLDER) == NULL)
	{
		if (mkdir(LOG_FOLDER, 0777) < 0)
		{
			logger->LogWarning("Looks like not enouph permissions");
			std::cout << "Looks like not enouph permissions" << std::endl;
			delete logger;
			delete sock;
			exit(EXIT_FAILURE);
		}
	}

	/*	Проверяю наличие Лок файла и создаю его если он не создан ранее  */
	if (IsLocked() == true) {
		logger->LogWarning("Already locked");
		std::cout << "Already locked" << std::endl;
		delete logger;
		delete sock;
		exit(-1);
	}
	if (Lock() == false) {
		logger->LogWarning("Cannot Lock");
		std::cout << "Cannot Lock" << std::endl;
		delete logger;
		delete sock;
		exit(-1);
	}

	/*	Демонизаци прораммы (делается форк, главная прога на этом заканчивает работу,
	**	потомок (демон) продолжает работу в качестве фонового процесса)  */
	switch (pid = fork()) {
	case -1:
		logger->LogError("Cannot create system process");
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

	if (sock->Dial(logger) < 0) {
		logger->LogError("Socket dial failed");
		Unlock();
		logger->LogInfo("Lock file was removed");
		delete logger;
		delete sock;
		exit(-1);
	}

	if (auth(sock, logger) < 0) {
		logger->LogError("Auth fail");
		Unlock();
		logger->LogInfo("Lock file was removed");
		delete logger;
		delete sock;
		exit(-1);
	}
	if (logLoop(sock, logger) < 0) {
		logger->LogError("Fail reading socket");
		Unlock();
		logger->LogInfo("Lock file was removed");
		delete logger;
		delete sock;
		exit(-1);
	}

	Unlock();
	logger->LogInfo("Lock file was removed");
	delete logger;
	delete sock;
	exit(0);
}

