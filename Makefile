# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/28 12:59:24 by vismaily          #+#    #+#              #
#    Updated: 2022/11/20 09:58:53 by vismaily         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

TMP			= Tmp

SRCS_COMM	= $(wildcard ./Srcs/*.cpp)
SRCS_PARS	= $(wildcard ./Srcs/Parsing/*.cpp)
SRCS_CLAS	= $(wildcard ./Includes/*.cpp)
SRCS		= $(SRCS_COMM) $(SRCS_PARS) $(SRCS_CLAS)

OBJS		= $(patsubst ./Srcs/%.cpp, ./$(TMP)/%.o, $(SRCS_COMM)) \
			  $(patsubst ./Srcs/Parsing/%.cpp, ./$(TMP)/%.o, $(SRCS_PARS)) \
			  $(patsubst ./Includes/%.cpp, ./$(TMP)/%.o, $(SRCS_CLAS)) \

CC			= c++

CFLAGS		= -Wall -Wextra -Werror -std=c++98

INCLUDES	= -I ./Includes

RM			= rm -rf

all:		$(NAME)

$(TMP):
			@$(shell mkdir $(TMP))

$(TMP)/%.o:	Srcs/%.cpp
			@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TMP)/%.o:	Srcs/Parsing/%.cpp
			@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(TMP)/%.o:	Includes/%.cpp
			@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(NAME):	$(TMP) $(OBJS)
			@$(CC) $(CFLAGS) $(INCLUDES) -o $(NAME) $(OBJS)

clean:
			@$(RM) $(TMP)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
