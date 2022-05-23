#ifndef TINTIN_REPORTER
# define TINTIN_REPORTER

# include <iostream>
# include "config.hpp"
# include "my_timer.hpp"
# include "utils.hpp"

class Tintin_reporter
{
	private:
		FILE			*file;
		myTimer 		timer;
		unsigned int	timestamp;
	public:
		void 		Log(const char *);
		void 		LogInfo(const char *);
		void 		LogWarning(const char *);
		void 		LogError(const char *);
		const char	*newFilePath();
		void		createLogFile();
		void		closeLogFile();
		bool		isNeedToChangeLogFile();
		void		changeLogFile();
		void		changeLogFileIfNeeded();

		/*	Конструктор  */
		Tintin_reporter()
		{
			/*	Зануляю все стартовые поля  */
			this->file = NULL;
			this->timestamp = 0;

			this->createLogFile();			
		};

		/*	Деструктор  */
		~Tintin_reporter()
		{
			this->closeLogFile();
		};
};

#endif