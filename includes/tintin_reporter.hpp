#ifndef TINTIN_REPORTER
# define TINTIN_REPORTER

# include <iostream>
# include "config.hpp"
# include "my_timer.hpp"
# include "utils.hpp"
# include "reporter.hpp"

class Tintin_reporter : public Reporter
{
	private:
		FILE			*file;
		myTimer 		timer;
		unsigned int	timestamp;
	public:
		void 		Log(const char *) override;
		void 		LogInfo(const char *) override;
		void 		LogWarning(const char *) override;
		void 		LogError(const char *) override;
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
		}

		/*	Деструктор  */
		~Tintin_reporter() override
		{
			this->closeLogFile();
		}
};

#endif