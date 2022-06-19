#include "locker.hpp"

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

bool	Lock()
{
	gLockFile = fopen(LOCK_FILE_PATH, "w");
	if (gLockFile == NULL) {
		return false;
	}
	return true;
}

void	Unlock()
{
	if (gLockFile != NULL)
	{
		fclose(gLockFile);
		remove(LOCK_FILE_PATH);
	}
}