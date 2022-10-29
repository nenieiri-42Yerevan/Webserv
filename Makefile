# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/28 12:59:24 by vismaily          #+#    #+#              #
#    Updated: 2022/10/29 16:56:30 by vismaily         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

SRCS		= $(wildcard ./Srcs/*.cpp) $(wildcard ./Includes/*.cpp)

OBJS		= $(patsubst ./Srcs/%.cpp, ./$(TMP)/%.o, $(SRCS))

TMP			= Tmp/

CC			= c++

CFLAGS		= -Wall -Wextra -Werror -std=c++98

INCLUDES	= -I ./Includes

RM			= rm -rf

all:		$(NAME)

$(TMP)/%.o:	srcs/%.cpp
			@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TMP)/%.o:	Includes/%.cpp
			@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(NAME):	$(TMP) $(OBJS)
			@$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS)

$(TMP):
			@mkdir $(TMP)

clean:
			@$(RM) $(TMP)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
