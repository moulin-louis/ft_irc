# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/13 13:30:11 by loumouli          #+#    #+#              #
#    Updated: 2023/02/16 12:54:18 by loumouli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ################################## #
#               COMMAND              #
# ################################## #
CC			= c++
MKDIR		= mkdir -p
RM			= rm -rf

# ################################## #
#              EXEC NAME             #
# ################################## #
NAME		= banbot

# ################################## #
#               SOURCES              #
# ################################## #
C_DIR		= 	src
C_FILES		=	main.cpp					\
				Banbot.cpp					\
				Parsing.cpp					\
				Connection.cpp				\
				routine.cpp					\

SRCS		= $(patsubst %, $(C_DIR)/%, $(C_FILES))

# ################################## #
#               OBJECTS              #
# ################################## #
O_DIR		= objs
O_FILES		= $(C_FILES:.cpp=.o)
OBJS		= $(patsubst %, $(O_DIR)/%, $(O_FILES))

# ################################## #
#                FLAGS               #
# ################################## #
CFLAGS		= -Wall -Wextra -Werror -g -MMD

# ################################## #
#                INCLUDES            #
# ################################## #
CINCLUDES	=	-I ./inc

# ################################## #
#                RULES               #
# ################################## #


all:	$(NAME)

$(O_DIR):
			$(MKDIR) $(O_DIR)

$(O_DIR)/%.o: $(C_DIR)/%.cpp
			$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@

$(NAME): $(O_DIR) $(OBJS)
			$(CC)  $(OBJS) $(CFLAGS) -o $@

# ################################## #
#                CLEAN               #
# ################################## #

clean:
			$(RM) ${NAME}
			$(RM) $(O_DIR)

fclean:	
			$(RM) ${NAME}
			$(RM) $(O_DIR)

re:			fclean all


.PHONY: all clean fclean re

-include ./objs/*.d

.NOTPARALLEL:fclean