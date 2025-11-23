NAME = minishell

SRCS =	$(shell find ./ -iname "*.c")
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Wunreachable-code -Ofast

LIBFTDIR = ../lib/libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS	:= 	-I ../include/ \
			-I $(LIBFTDIR)

all: libft $(NAME)

libft:
	$(MAKE) -C $(LIBFTDIR)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) $(HEADERS) -o $(NAME) $(LIBFT)

clean:
	@rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	@rm -rf $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all, libft, clean, fclean, re