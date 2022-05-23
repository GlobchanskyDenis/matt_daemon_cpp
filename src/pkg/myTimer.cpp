#include "my_timer.hpp"

const char	*myTimer::FormatDateTime()
{
	time(&this->timer);
	this->tm_info = localtime(&(this->timer));
	strftime(this->buffer, 22, "%d/%m/%Y-%H:%M:%S", this->tm_info);
	return this->buffer;
}

const char	*myTimer::FormatDateTimeWithoutSeconds()
{
	time(&this->timer);
	this->tm_info = localtime(&(this->timer));
	strftime(this->buffer, 22, "%d_%m_%Y-%H:%M", this->tm_info);
	return this->buffer;
}

const char	*myTimer::FormatDateTimeWithoutMinutes()
{
	time(&this->timer);
	this->tm_info = localtime(&(this->timer));
	strftime(this->buffer, 22, "%d_%m_%Y-%H", this->tm_info);
	return this->buffer;
}

unsigned int	myTimer::GetTimestampTrimSeconds()
{
	time(&this->timer);
	this->tm_info = localtime(&(this->timer));
	return (unsigned int)(this->tm_info->tm_min) + (unsigned int)(this->tm_info->tm_hour * 60);
}

unsigned int	myTimer::GetTimestampTrimMinutes()
{
	time(&this->timer);
	this->tm_info = localtime(&(this->timer));
	return (unsigned int)(this->tm_info->tm_hour);
}
