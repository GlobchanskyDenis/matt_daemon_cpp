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

#include <sys/wait.h>
/*	Это логика работы супервизора который следит за остановкой дочернего процесса
**	и по его завершению логгирует и корректно завершает работу  */
void shutdownSupervisorLoop(Tintin_reporter* logger, int pid) {
	int status;

	do {
		if (waitpid(pid, &status, WUNTRACED | WCONTINUED) == -1) {
			logger->LogError("waitpid returned error");
			break ;
		}
		if (WIFEXITED(status)) {
			logger->LogInfo("exited");
		} else if (WIFSIGNALED(status)) {
			logger->LogInfo("killed by signal");
		} else if (WIFSTOPPED(status)) {
			logger->LogInfo("stopped by signal");
		} else if (WIFCONTINUED(status)) {
			logger->LogInfo("continued");
		}
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));

	Unlock();
	logger->LogInfo("Lock file was removed");
	delete logger;
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
	**	потомок (демон) продолжает работу в качестве фонового процесса)
	**	Цель данного форка - вернуть пользователю терминал  */
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
		std::cout << "Программа создает два системных процесса (shutdown supervisor + socket supervisor)" << std::endl;
		std::cout << "Gracefull shutdown supervisor pid " << pid << std::endl;
		std::cout << "Вышеобозначенный пид убивать утилитой kill не нужно т.к. он отслеживает состояние процесса socket supervisor" << std::endl;
		std::cout << "и сам завершит работу при завершении работы данного процесса (предварительно уведомив пользователя записью в лог файле)" << std::endl;
		exit(0);
	}

	/*	Вторая демонизация программы (цель - выполнить механизм gracefull shutdown)
	**	Процесс-родитель будет ответственен за закрытие файла лога,
	**	а процесс-потомок будет заниматься работой с сокетом и будет ответственен за его закрытие  */
	switch (pid = fork()) {
	case -1:
		logger->LogError("Cannot create system process");
		delete logger;
		delete sock;
		exit(-1);
	case 0:
		/*	Это процесс - потомок (демон) socket supervisor  */
		break ;
	default:
		/*	Это - процесс - родитель shutdown supervisor  */
		shutdownSupervisorLoop(logger, pid);
		exit(0);
	}

	/*	Дальше код работы с сокетом  */
	if (sock->Dial(logger) < 0) {
		logger->LogError("Socket dial failed");
		// Unlock();
		// logger->LogInfo("Lock file was removed");
		// delete logger;
		delete sock;
		exit(-1);
	}

	if (auth(sock, logger) < 0) {
		logger->LogError("Auth fail");
		// Unlock();
		// logger->LogInfo("Lock file was removed");
		// delete logger;
		delete sock;
		exit(-1);
	}
	if (logLoop(sock, logger) < 0) {
		logger->LogError("Fail reading socket");
		// Unlock();
		// logger->LogInfo("Lock file was removed");
		// delete logger;
		delete sock;
		exit(-1);
	}

	delete sock;
	exit(0);
}

