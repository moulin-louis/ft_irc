# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: armendi <armendi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/13 13:30:11 by loumouli          #+#    #+#              #
#    Updated: 2023/03/30 15:02:01 by armendi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############
## Variables ##
###############

NAME		=	ircserv

## add library here (minilibx, ncurses, libft, ...)
LIBDIR		=

INCLDIR		=	$(addsuffix /inc,$(LIBDIR) .)
BUILDIR		=	build
DEPDIR		=	$(BUILDIR)/.deps

SRCS_DIR	=	$(sort $(dir $(wildcard src/*/)))

SRC			=	$(addprefix src/,		\
						main.cpp		\
					$(addprefix channel/,\
						Channel.cpp		\
					)					\
					$(addprefix client/,\
						Client.cpp		\
						getter.cpp		\
						setter.cpp		\
					)					\
					$(addprefix commands/,\
						join.cpp		\
						kick.cpp		\
						kill.cpp		\
						list.cpp		\
						mode.cpp		\
						motd.cpp		\
						names.cpp		\
						nick.cpp		\
						oper.cpp		\
						pass.cpp		\
						ping.cpp		\
						private_msg.cpp	\
						quit.cpp		\
						restart.cpp		\
						topic.cpp 		\
						user.cpp		\
						part.cpp		\
						wallops.cpp		\
						who.cpp			\
						away.cpp		\
					)					\
					$(addprefix server/,\
						chan_management.cpp\
						connection.cpp	\
						init_server.cpp	\
						handle_input.cpp\
						server_tools.cpp\
						Server.cpp		\
					)					\
					$(addprefix tools/,	\
   						Tools.cpp		\
					)					\
				)

OBJ			=	$(SRC:%.cpp=$(BUILDIR)/%.o)
DEP			=	$(SRC:%.cpp=$(DEPDIR)/%.d)

CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g -O2
CPPFLAGS	=	$(addprefix -I, $(INCLDIR))
LDFLAGS		=
DEPFLAGS	=	-MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

RM			=	/bin/rm -rf

# FG COLORS
DEFAULT		=	\033[0m
BLACK		=	\033[1;30m
RED			=	\033[1;31m
GREEN		=	\033[1;32m
YELLOW		=	\033[1;33m
BLUE		=	\033[1;34m
MAGENTA 	=	\033[1;35m
CYAN 		=	\033[1;36m
WHITE 		=	\033[1;107m

# TERMCAPS
UP			=	\033[1A
DELETE		=	\033[2K
DELPREV		=	$(UP)$(DELETE)\r

# EMOJI
CHECK		=	\xE2\x9C\x94
CROSS		=	\xE2\x9D\x8C

# DISPLAY
HEAD_SIZE	=	32
NAME_SIZE	=	$(shell NAME='$(NAME)'; printf "$${\#NAME}")
PAD_WIDTH	=	$(shell printf "$$((($(HEAD_SIZE) - $(NAME_SIZE)) / 2))")
PAD_PREC	=	$(shell printf "$$(($(PAD_WIDTH) / 2))")
PAD_CHAR	=	\*
PAD_STR		=	$(shell printf '$(PAD_CHAR)%.0s' {1..$(PAD_WIDTH)})
LEFT_PAD	=	$(shell printf '%-*.*s' $(PAD_WIDTH) $(PAD_PREC) $(PAD_STR))
RIGHT_PAD	=	$(shell printf '%*.*s' $$(($(PAD_WIDTH) + $(NAME_SIZE) % 2)) $(PAD_PREC) $(PAD_STR))
BODY_WIDTH	=	$(shell printf "$$(($(HEAD_SIZE) - 1))")


###########
## Rules ##
###########

.PHONY:			all bonus clean fclean header lib re verbose

all:
				@$(MAKE) -s $(NAME)

bonus:
				$(info Building bot...)
				@$(MAKE) -C ./bot
				@$(MAKE) -s $(NAME)

$(BUILDIR)/%.o:	%.cpp | $(DEPDIR)
				@mkdir -p build/ $(addprefix build/, $(SRCS_DIR))
				@mkdir -p build/.deps/ $(addprefix build/.deps/, $(SRCS_DIR))
				@printf "$(YELLOW)Compiling $(BLUE)$@$(DEFAULT)$(YELLOW) and generating/checking make dependency file...$(DEFAULT)\n"
				$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
				@printf '$(DELPREV)%-*s$(GREEN)$(CHECK)$(DEFAULT)\n' $(BODY_WIDTH) $(notdir $@)

$(NAME):		$(OBJ)
				printf "$(YELLOW)Linking source files and generating $(CYAN)$@$(DEFAULT) $(YELLOW)binary...\n$(DEFAULT)"
				$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)
				printf "$(DELPREV)$(GREEN)Binary generated$(DEFAULT)\n"

$(DEPDIR):
				@printf "$(YELLOW)Creating $(BLUE)$@$(DEFAULT)$(YELLOW) folder...$(DEFAULT)\n"
				mkdir -p $@
				@printf "$(DELPREV)"
$(DEP):
-include $(wildcard $(DEP))

clean:
				@printf "$(YELLOW)BOT: Deleting object and dependency files...$(DEFAULT)\n"
				$(MAKE) -C ./bot clean
				@printf "$(YELLOW)Deleting object and dependency files...$(DEFAULT)\n"
				$(RM) $(OBJ)
				@printf "$(DELPREV)Build files deleted\n"

fclean:			clean
				@printf "$(YELLOW)BOT: Deleting build directory and binary...$(DEFAULT)\n"
				$(MAKE) -C ./bot fclean
				@printf "$(YELLOW)Deleting build directory and binary...$(DEFAULT)\n"
				$(RM) $(BUILDIR) $(NAME)
				@printf "$(DELPREV)Build directory and binary deleted\n"

re:				fclean
				@$(MAKE) -s all

rebonus:		fclean
				@$(MAKE) -s bonus


