#ifndef UTILS
# define UTILS

#include <cstring>
#include <cstdlib>
#include "md5.hpp"

const char	*strjoin(const char *s1, const char *s2);
const char	*strjoin(const char *s1, const char *s2, const char *s3);
char*  		strnew(int len);
void		trim(char* src, int length);
bool		isEqual(const char* s1, const char* s2, int length);
const char*		hash_this(const char* s);

#endif