CC				=	g++
FLAGS			=	-Wall -Wextra -Werror
INCLUDES_DIR	=	includes

MATT_DAEMON_BIN			=	Matt_daemon
MATT_DAEMON_DIR			=	src/cmd/matt_daemon
MATT_DAEMON_FILES		=	main.cpp		auth.cpp
MATT_DAEMON_FILENAMES	=	$(addprefix $(MATT_DAEMON_DIR)/,$(MATT_DAEMON_FILES))
MATT_DAEMON_OBJ			=	$(addprefix $(MATT_DAEMON_DIR)/,$(MATT_DAEMON_FILES:.c=.o))

PKG_DIR			=	src/pkg
PKG_FILES		=	tintinReporter.cpp			locker.cpp			\
					myTimer.cpp				utils.cpp			\
					socket.cpp				md5.cpp
PKG_FILENAMES	=	$(addprefix $(PKG_DIR)/,$(PKG_FILES))
PKG_OBJ			=	$(addprefix $(PKG_DIR)/,$(PKG_FILES:.c=.o))


.PHONY: all clean fclean re

all : $(MATT_DAEMON_BIN)

$(MATT_DAEMON_BIN) : $(MATT_DAEMON_FILENAMES)
	@echo "компилирую бинарник демона"
	@$(CC) $(FLAGS) -o $(MATT_DAEMON_BIN) $(MATT_DAEMON_OBJ) $(PKG_OBJ) -I $(INCLUDES_DIR)



fclean: clean
	@echo "удаляю бинарники"
	@rm -rf $(MATT_DAEMON_BIN)

re: fclean all