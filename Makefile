# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/12 17:35:31 by vgoyzuet          #+#    #+#              #
#    Updated: 2025/03/15 21:23:24 by vgoyzuet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN	=	\033[1;32m
CYAN	=	\033[0;36m
WHITE	=	\033[0m
CLEAR	=	\r\033[K

NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror -I libft
CFLAGS += -I inc

LIBFT = libft/libft.a

SRCS = 	src/main.c\

OBJS = $(SRCS:%.c=objs/%.o)

all: $(LIBFT) $(NAME)

$(NAME): objs $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "$(GREEN)pipex ready$(WHITE)"

$(LIBFT):
	@make --silent -C libft

objs:
	@mkdir -p objs/src/

objs/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make --silent clean -C libft
	@rm -rf objs
	@echo "Objetcs files deleted."

fclean: clean
	@make --silent fclean -C libft
	@rm -f $(NAME)
	@echo "Full clean completed"

re: fclean all

.PHONY: all clean fclean re
