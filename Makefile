# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/25 05:11:56 by slahrach          #+#    #+#              #
#    Updated: 2023/04/01 22:25:10 by mchliyah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

CC = g++

CFLAGS = -Wall -Wextra -Werror

STDFLAGS = -std=c++98

FILES = main.cpp \
	./parse/parse.cpp ./parse/utils.cpp ./parse/locationconfig.cpp ./parse/serverconfig.cpp \
	./request/server.cpp ./request/client.cpp \
	./response/response.cpp ./response/get_response.cpp ./response/post_response.cpp ./response/delete_response.cpp response/utils.cpp

OBJECTS = $(FILES:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS)  $(STDFLAGS) -c -o $@ $<

all : $(NAME)

$(NAME) :  $(OBJECTS)
	$(CC)   $(OBJECTS) -o $(NAME) #-fsanitize=address 


clean :
	$(RM) $(OBJECTS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean re fclean