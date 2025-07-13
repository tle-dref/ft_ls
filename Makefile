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
	@mkdir -p test_dir/subdir test_dir/empty_dir
	@touch test_dir/file1.txt test_dir/file2.c test_dir/.hidden test_dir/subdir/nested.txt test_dir/executable
	@chmod 755 test_dir/executable
	@chmod 644 test_dir/file2.c
	@chmod 777 test_dir/subdir
	@sleep 1 && touch test_dir/newer_file
	@ln -sf file1.txt test_dir/symlink
	@echo -n "Basic listing: " && (ls test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls):"; cat /tmp/ls_out; echo "--- Got (ft_ls):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Long format (-l): " && (ls -l test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -l test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -l):"; cat /tmp/ls_out; echo "--- Got (ft_ls -l):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Show hidden (-a): " && (ls -a test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -a test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -a):"; cat /tmp/ls_out; echo "--- Got (ft_ls -a):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Reverse order (-r): " && (ls -r test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -r test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -r):"; cat /tmp/ls_out; echo "--- Got (ft_ls -r):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Sort by time (-t): " && (ls -t test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -t test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -t):"; cat /tmp/ls_out; echo "--- Got (ft_ls -t):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Recursive (-R): " && (ls -R test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -R test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -R):"; cat /tmp/ls_out; echo "--- Got (ft_ls -R):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -la: " && (ls -la test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -la test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -la):"; cat /tmp/ls_out; echo "--- Got (ft_ls -la):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -lrt: " && (ls -lrt test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -lrt test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -lrt):"; cat /tmp/ls_out; echo "--- Got (ft_ls -lrt):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -lar: " && (ls -lar test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -lar test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -lar):"; cat /tmp/ls_out; echo "--- Got (ft_ls -lar):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -lart: " && (ls -lart test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -lart test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -lart):"; cat /tmp/ls_out; echo "--- Got (ft_ls -lart):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -ltR: " && (ls -ltR test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -ltR test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -ltR):"; cat /tmp/ls_out; echo "--- Got (ft_ls -ltR):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -aR: " && (ls -aR test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -aR test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -aR):"; cat /tmp/ls_out; echo "--- Got (ft_ls -aR):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -arR: " && (ls -arR test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -arR test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -arR):"; cat /tmp/ls_out; echo "--- Got (ft_ls -arR):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Combined -lartR: " && (ls -lartR test_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -lartR test_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -lartR):"; cat /tmp/ls_out; echo "--- Got (ft_ls -lartR):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Current directory: " && (ls > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls):"; cat /tmp/ls_out; echo "--- Got (ft_ls):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Current dir -la: " && (ls -la > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -la > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -la):"; cat /tmp/ls_out; echo "--- Got (ft_ls -la):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Multiple args: " && (ls test_dir/file1.txt test_dir/subdir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls test_dir/file1.txt test_dir/subdir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls):"; cat /tmp/ls_out; echo "--- Got (ft_ls):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Multiple args -l: " && (ls -l test_dir/file1.txt test_dir/subdir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls -l test_dir/file1.txt test_dir/subdir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls -l):"; cat /tmp/ls_out; echo "--- Got (ft_ls -l):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Empty directory: " && (ls test_dir/empty_dir > /tmp/ls_out 2>/tmp/ls_err && ./ft_ls test_dir/empty_dir > /tmp/ft_ls_out 2>/tmp/ft_ls_err && diff /tmp/ls_out /tmp/ft_ls_out > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected (ls):"; cat /tmp/ls_out; echo "--- Got (ft_ls):"; cat /tmp/ft_ls_out; echo "--- Errors:"; cat /tmp/ls_err /tmp/ft_ls_err 2>/dev/null || true; echo))
	@echo -n "Non-existent file: " && (ls non_existent 2> /tmp/ls_err >/dev/null; ./ft_ls non_existent 2> /tmp/ft_ls_err >/dev/null; diff /tmp/ls_err /tmp/ft_ls_err > /dev/null && echo "\033[32mSUCCESS\033[0m" || (echo "\033[31mFAILURE\033[0m"; echo "--- Expected error (ls):"; cat /tmp/ls_err; echo "--- Got error (ft_ls):"; cat /tmp/ft_ls_err; echo))
	@rm -rf test_dir /tmp/ls_out /tmp/ft_ls_out /tmp/ls_err /tmp/ft_ls_err
	@echo "All tests completed!"

test_clean:
	@rm -rf test_dir

.PHONY: all clean fclean re test test_clean
