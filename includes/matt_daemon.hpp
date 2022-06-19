#ifndef MATT_DAEMON
# define MATT_DAEMON

# include <iostream>
# include <dirent.h>
# include <sys/stat.h>
# include "tintin_reporter.hpp"
# include "reporter.hpp"
# include "locker.hpp"
# include "socket.hpp"
# include "utils.hpp"

int	auth(Socket* sock, Tintin_reporter* logger);

#endif