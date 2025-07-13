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

# Test rule
test: $(NAME)
	@echo "Testing ft_ls against system ls..."
	@mkdir -p test_dir/subdir
	@touch test_dir/file1.txt test_dir/file2.c test_dir/.hidden test_dir/subdir/nested.txt
	@chmod 755 test_dir/file1.txt
	@chmod 644 test_dir/file2.c
	@echo "=== Test 1: Basic listing ==="
	@echo "System ls:"
	@ls test_dir
	@echo "Our ft_ls:"
	@./ft_ls test_dir
	@echo
	@echo "=== Test 2: Long format (-l) ==="
	@echo "System ls -l:"
	@ls -l test_dir
	@echo "Our ft_ls -l:"
	@./ft_ls -l test_dir
	@echo
	@echo "=== Test 3: Show hidden files (-a) ==="
	@echo "System ls -a:"
	@ls -a test_dir
	@echo "Our ft_ls -a:"
	@./ft_ls -a test_dir
	@echo
	@echo "=== Test 4: Reverse order (-r) ==="
	@echo "System ls -r:"
	@ls -r test_dir
	@echo "Our ft_ls -r:"
	@./ft_ls -r test_dir
	@echo
	@echo "=== Test 5: Sort by time (-t) ==="
	@echo "System ls -t:"
	@ls -t test_dir
	@echo "Our ft_ls -t:"
	@./ft_ls -t test_dir
	@echo
	@echo "=== Test 6: Combined flags (-la) ==="
	@echo "System ls -la:"
	@ls -la test_dir
	@echo "Our ft_ls -la:"
	@./ft_ls -la test_dir
	@echo
	@echo "=== Test 7: Combined flags (-lrt) ==="
	@echo "System ls -lrt:"
	@ls -lrt test_dir
	@echo "Our ft_ls -lrt:"
	@./ft_ls -lrt test_dir
	@echo
	@echo "=== Test 8: Current directory ==="
	@echo "System ls:"
	@ls
	@echo "Our ft_ls:"
	@./ft_ls
	@echo
	@echo "=== Test 9: Non-existent file ==="
	@echo "System ls:"
	@ls non_existent_file 2>&1 || true
	@echo "Our ft_ls:"
	@./ft_ls non_existent_file 2>&1 || true
	@rm -rf test_dir
	@echo "Tests completed!"

test_clean:
	@rm -rf test_dir

.PHONY: all clean fclean re test test_clean
