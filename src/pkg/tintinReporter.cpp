#include "tintin_reporter.hpp"

void	Tintin_reporter::Log(const char *log_message)
{
	int		amount_logged;

	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	amount_logged = fprintf(this->file, "[%s] [ LOG ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
	if (amount_logged <= 0) {
		std::cout << "Fail: amount logged " << amount_logged << std::endl;
	}
}

void	Tintin_reporter::LogInfo(const char *log_message)
{
	int		amount_logged;

	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	amount_logged = fprintf(this->file, "[%s] [ INFO ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
	if (amount_logged <= 0) {
		std::cout << "Fail: amount logged " << amount_logged << std::endl;
	}
}

void	Tintin_reporter::LogWarning(const char *log_message)
{
	int		amount_logged;

	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	amount_logged = fprintf(this->file, "[%s] [ WARNING ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
	if (amount_logged <= 0) {
		std::cout << "Fail: amount logged " << amount_logged << std::endl;
	}
}

void	Tintin_reporter::LogError(const char *log_message)
{
	int		amount_logged;

	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	amount_logged = fprintf(this->file, "[%s] [ ERROR ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
	if (amount_logged <= 0) {
		std::cout << "Fail: amount logged " << amount_logged << std::endl;
	}
}

const char	*Tintin_reporter::newFilePath()
{
	if (CHANGE_FILE_EVERY_MINUTE) {
		this->timestamp = this->timer.GetTimestampTrimSeconds();
		return strjoin(LOG_FOLDER, this->timer.FormatDateTimeWithoutSeconds(), ".log");
	} else {
		this->timestamp = this->timer.GetTimestampTrimMinutes();
		return strjoin(LOG_FOLDER, this->timer.FormatDateTimeWithoutMinutes(), ".log");
	}
}

void	Tintin_reporter::changeLogFileIfNeeded()
{
	if (this->isNeedToChangeLogFile())
	{
		this->changeLogFile();
	}
}

void	Tintin_reporter::changeLogFile()
{
	this->closeLogFile();
	this->createLogFile();
}

void	Tintin_reporter::createLogFile()
{
	/*	?????????????????? ???????? ?????? ???????????? ??????????
	**	a+  -- ???????????????????? ???????????????????? ?? ?????????? ?????????? ?????? ?????????????? ????????  */
	if (CHANGE_FILE_EVERY_MINUTE) {
		this->timestamp = this->timer.GetTimestampTrimSeconds();
	} else {
		this->timestamp = this->timer.GetTimestampTrimMinutes();
	}
	const char *filePath = this->newFilePath();
	this->file = fopen(filePath, "a+b");
	if (this->file == NULL)
	{
		std::cout << "Error: cant open file " << filePath << std::endl;
		std::cout << "Looks like not enouph permissions" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	Tintin_reporter::closeLogFile()
{
	if (this->file != NULL)
	{
		fclose(this->file);
	}
}

bool	Tintin_reporter::isNeedToChangeLogFile()
{
	if (CHANGE_FILE_EVERY_MINUTE) {
		if (this->timestamp != this->timer.GetTimestampTrimSeconds()) {
			return true;
		}
	} else {
		if (this->timestamp != this->timer.GetTimestampTrimMinutes()) {
			return true;
		}
	}
	
	return false;
}