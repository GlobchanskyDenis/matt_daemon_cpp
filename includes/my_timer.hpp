#ifndef MY_TIMER
# define MY_TIMER

# include <iostream>

class myTimer
{
	private:
		time_t		timer;
		char	    buffer[22];
		struct tm*	tm_info;
	public:
		const char		*FormatDateTime();
		const char		*FormatDateTimeWithoutSeconds();
		const char		*FormatDateTimeWithoutMinutes();
		unsigned int	GetTimestampTrimSeconds();
		unsigned int	GetTimestampTrimMinutes();
};

#endif