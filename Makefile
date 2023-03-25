# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/25 05:11:56 by slahrach          #+#    #+#              #
#    Updated: 2023/03/25 17:12:20 by slahrach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

CC = c++

CFLAGS = -Wall -Wextra -Werror

STDFLAGS = -std=c++98

FILES = main.cpp \
	./parse/parse.cpp ./parse/utils.cpp ./parse/locationconfig.cpp ./parse/serverconfig.cpp \
	./request/server.cpp ./request/client.cpp \
	./response/response.cpp ./response/get_response.cpp ./response/post_response.cpp ./response/delete_response.cpp response/utils.cpp

OBJECTS = $(FILES:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

all : $(NAME)

$(NAME) :  $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) -fsanitize=address 


clean :
	$(RM) $(OBJECTS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean re fclean