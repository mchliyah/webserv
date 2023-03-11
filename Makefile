# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/25 05:11:56 by slahrach          #+#    #+#              #
#    Updated: 2023/03/11 23:41:50 by mchliyah         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

CC = c++

CFLAGS = -Wall -Wextra -Werror

STDFLAGS = -std=c++98

SRCS = main.cpp server.cpp client.cpp parse.cpp utils.cpp locationconfig.cpp serverconfig.cpp 
OBJS = ${SRCS:.cpp=.o}

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${STDFLAGS} ${OBJS} -o ${NAME}

clean :
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

%.o : %.cpp
	${CC} ${STDFLAGS} -c $<

.PHONY: clean all fclean re