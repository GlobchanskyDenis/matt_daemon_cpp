#include "utils.hpp"

const char	*strjoin(const char *s1, const char *s2)
{
	char*		result;

	result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
	if (result)
	{
		strcpy(result, s1);
		strcat(result, s2);
	}
	return result;
}

const char	*strjoin(const char *s1, const char *s2, const char *s3)
{
	char*		result;

	result = (char *)malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1);
	if (result)
	{
		strcpy(result, s1);
		strcat(result, s2);
		strcat(result, s3);
	}
	return result;
}

char*   strnew(int len) {
	char *dst = (char *)malloc(sizeof(char) * len);
	int i;
	for (i = 0; i < len; i++) {
		dst[i] = 0;
	}
	return dst;
}


void	trim(char* src, int length) {
	int	start, end;
	for (start = 0; start < length; start++) {
		if (src[start] != ' ' && src[start] != '\t' && src[start] != '\n') {
			break ;
		}
	}
	for (end = length - 1; end > 0; end--) {
		if (src[end] != ' ' && src[end] != '\t' && src[end] != '\n' && src[end] != 0) {
			break ;
		}
	}
	if (start >= end) {
		return ;
	}
	strncpy(src, &src[start], length - start);
	int i;
	for (i = end - start + 1; i < length; i++) {
		src[i] = 0;
	}
}

bool	isEqual(const char* s1, const char* s2, int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (s1[i] == '\0' && s2[i] == '\0') {
			return true;
		}
		if (s1[i] != s2[i]) {
			return false;
		}
	}
	return true;
}

const char*	hash_this(const char* s) {
	std::string hashed = md5( s );
	const char* c_hashed = hashed.c_str();
	char* dst = strnew(strlen(c_hashed));
	strcpy(dst, c_hashed);
	return dst;
}
