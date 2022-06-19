#ifndef REPORTER
# define REPORTER

class Reporter
{
	public:
		virtual void	Log(const char *){}
		virtual void	LogInfo(const char *){}
		virtual void	LogWarning(const char *){}
		virtual void	LogError(const char *){}

		virtual ~Reporter(){}
};

#endif