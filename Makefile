# Variables
NAME = ft_ls
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIE -g

# Lib
LIB_GIT = git@github.com:tle-dref/koalib.git
LIB_DIR = koalib
LIB_A = $(LIB_DIR)/libft.a
LDFLAGS = -L$(LIB_DIR) -lft -pie
CPPFLAGS = -I$(LIB_DIR)/includes -Iinc

# Sources
SRCS_DIR = src
SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Rules
all: $(NAME)

lib: $(LIB_A)

$(NAME): $(OBJS) $(LIB_A)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

%.o: %.c $(LIB_A)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB_A):
	@if [ ! -d "$(LIB_DIR)" ]; then \
		echo "Cloning $(LIB_GIT)..."; \
		git clone $(LIB_GIT) $(LIB_DIR); \
	fi
	@if [ ! -f "$(LIB_A)" ]; then \
		echo "Building library..."; \
		$(MAKE) -C $(LIB_DIR) CFLAGS="-Wall -Wextra -Werror -fPIC -g -I includes"; \
	fi

clean:
	rm -f $(OBJS)
	@if [ -d "$(LIB_DIR)" ]; then \
		$(MAKE) -C $(LIB_DIR) clean; \
	fi

fclean: clean
	rm -f $(NAME)
	@if [ -d "$(LIB_DIR)" ]; then \
		$(MAKE) -C $(LIB_DIR) fclean; \
		rm -rf $(LIB_DIR); \
	fi

re: fclean all

.PHONY: all clean fclean re
