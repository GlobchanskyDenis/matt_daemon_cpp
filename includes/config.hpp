#ifndef CONFIG
# define CONFIG

/*	Logger  */
# define DAEMON_NAME				"Matt_daemon"
# define LOG_FOLDER					"/var/log/matt_daemon/" //"/var/log/matt_daemon/"
# define CHANGE_FILE_EVERY_MINUTE	false

/*	Locker  */
# define LOCK_FILE_PATH	"/var/lock/matt_daemon.lock"

/*	Authenticate  */
# define SERVER_LOGIN			"bsabre-c"
# define SERVER_PASSWORD_HASH	"5f4dcc3b5aa765d61d8327deb882cf99" // "password"

/*	Socket  */
# define IS_TCP_SOCKET				true
# define IP							"127.0.0.1"
# define PORT						4242
# define MAX_SOCKET_MESSAGE_LENGTH	1024

#endif