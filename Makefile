NAME = minishell

SRCS =	$(shell find ./ -iname "*.c")
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Werror -g3 -I include/ 
LDFLAGS = -lreadline

LIBFTDIR = lib/libft
LIBFT = $(LIBFTDIR)/libft.a

HEADERS	:= 	-I ../include/ \
			-I $(LIBFTDIR)

all: libft $(NAME)

libft:
	$(MAKE) -C $(LIBFTDIR)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(HEADERS) $(OBJS) -o $(NAME) $(LIBFT)

clean:
	@rm -rf $(OBJS)
	$(MAKE) -C $(LIBFTDIR) clean


fclean: clean
	@rm -rf $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean


re: fclean all

.PHONY: all clean fclean re