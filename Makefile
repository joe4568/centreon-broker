##
## Makefile for Centreon-Broker in .
## 
## Made by Matthieu Kermagoret <mkermagoret@merethis.com>
## 
## Copyright Merethis
## See LICENSE file for details.
## 
## Started on  05/04/09 Matthieu Kermagoret
## Last update 05/11/09 Matthieu Kermagoret
##

CXX		=	g++
RM		=	rm -rf

SRC		=	src/conditionvariable.cpp	\
			src/event.cpp			\
			src/eventpublisher.cpp		\
			src/eventsubscriber.cpp		\
			src/exception.cpp		\
			src/fileoutput.cpp		\
			src/hoststatusevent.cpp		\
			src/mutex.cpp			\
			src/mysqloutput.cpp		\
			src/networkinput.cpp		\
			src/servicestatusevent.cpp	\
			src/thread.cpp
OBJ		=	$(SRC:.cpp=.o)

NAME		=	centreon-broker.so

INCLUDE		+=	-Iinc
CXXFLAGS	+=	--std=c++0x -W -Wall -pedantic $(INCLUDE)	\
			 `mysql_config --cflags --include`
LDFLAGS		+=	-lpthread `mysql_config --libs`


all		:	$(NAME)

$(NAME)		:	$(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)

clean		:
	$(RM) `find . -name '*~' -or -name '#*#'`  $(OBJ)

fclean		:	clean
	$(RM) $(NAME)

re		:	fclean all

.PHONY		:	all clean fclean re
