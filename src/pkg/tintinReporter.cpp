#include "tintin_reporter.hpp"

void	Tintin_reporter::Log(const char *log_message)
{
	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	fprintf(this->file, "[%s] [ LOG ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
}

void	Tintin_reporter::LogInfo(const char *log_message)
{
	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	fprintf(this->file, "[%s] [ INFO ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
}

void	Tintin_reporter::LogWarning(const char *log_message)
{
	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	fprintf(this->file, "[%s] [ WARNING ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
}

void	Tintin_reporter::LogError(const char *log_message)
{
	if (this->file == NULL)
	{
		std::cout << "Fail: log file not exist" << std::endl;
		return ;
	}
	this->changeLogFileIfNeeded();
	fprintf(this->file, "[%s] [ ERROR ] - %s: %s\n", this->timer.FormatDateTime(), DAEMON_NAME, log_message);
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
	/*	открывает файл для записи логов
	**	a+  -- дописывает информацию к концу файла или создает файл  */
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