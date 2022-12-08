# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/28 12:59:24 by vismaily          #+#    #+#              #
#    Updated: 2022/12/08 19:30:46 by vismaily         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

TMP			= Tmp

SRCS_COMM	= $(wildcard ./Srcs/*.cpp)
SRCS_PARS	= $(wildcard ./Srcs/Parsing/*.cpp)
SRCS		= $(SRCS_COMM) $(SRCS_PARS)

OBJS		= $(patsubst ./Srcs/%.cpp, ./$(TMP)/%.o, $(SRCS_COMM)) \
			  $(patsubst ./Srcs/Parsing/%.cpp, ./$(TMP)/%.o, $(SRCS_PARS)) \

CPP			= c++

CPP_FLAGS	= -Wall -Wextra -Werror -std=c++98

BONUS_PART	= 0

DEFINES		= -D BONUS=$(BONUS_PART)

INCLUDES	= -I ./Includes

RM			= rm -rf

$(NAME):	$(TMP) $(OBJS)
			@$(CPP) -o $(NAME) $(OBJS)

$(TMP):
			@$(shell mkdir $(TMP))

$(TMP)/%.o:	Srcs/%.cpp
			@$(CPP) $(CPP_CFLAGS) $(DEFINES) $(INCLUDES) -o $@ -c $<

$(TMP)/%.o:	Srcs/Parsing/%.cpp
			@$(CPP) $(CPP_CFLAGS) $(DEFINES) $(INCLUDES) -o $@ -c $<

bonus:		bonus_on $(NAME)

bonus_on:
			$(eval BONUS_PART = 1)

all:		bonus

clean:
			@$(RM) $(TMP)

fclean:		clean
			@$(RM) $(NAME) temp

re:			fclean $(NAME)

.PHONY:		all clean fclean re bonus
