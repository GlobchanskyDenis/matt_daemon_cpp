#include "locker.hpp"

// const char	*gLockFilePath = "matt_daemon.lock";
FILE		*gLockFile;

bool	IsLocked(void)
{
	FILE		*tmpLockFile;

	/*	Открываю файл "только для чтения"  */
	tmpLockFile = fopen(LOCK_FILE_PATH, "r");
	if (tmpLockFile == NULL)
	{
		return false;
	}
	fclose(tmpLockFile);
	return true;
}

void	Lock()
{
	gLockFile = fopen(LOCK_FILE_PATH, "r");
}

void	Unlock()
{
	if (gLockFile != NULL)
	{
		fclose(gLockFile);
		remove(LOCK_FILE_PATH);
	}
}