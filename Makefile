# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/09 10:30:11 by jgoldste          #+#    #+#              #
#    Updated: 2024/02/03 20:30:45 by jgoldste         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME		:= webserv

SRCDIR		:= src
OBJDIR		:= object
HDRDIR		:= include
INC			:= -I./include

CXX			:= g++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -g

HDRS		:= $(wildcard $(HDRDIR)/*.hpp $(HDRDIR)/*.tpp $(HDRDIR)/*.ipp)
SRCS		:= $(wildcard *.cpp $(SRCDIR)/*.cpp)
OBJS		:= $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS		:= $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

MKDIR		:= mkdir -p
RM			:= rm -rf

DSYM		:= $(wildcard *.dSYM)

# Define color codes for output messages
YELLOW		:= "\033[1;33m"
GREEN		:= "\033[1;32m"
END			:= "\033[0m"

# Declare phony targets
.PHONY: all clean fclean re

# Default target for building the project
all: $(NAME)

# Main target
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo $(GREEN) "\n\tProject is compiled\n" ${END}

# Rule for generating dependency and object files
$(OBJDIR)/%.o: ./%.cpp $(HDRS) Makefile
	$(MKDIR) $(@D)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $< -MD

# Target for cleaning the project
clean:
	$(RM) $(OBJDIR) $(DSYM)
	@echo $(YELLOW) "\n\tProject 'clean' status: DONE\n" ${END}

# Target for fully cleaning the project
fclean: clean
	$(RM) $(NAME)
	@echo $(YELLOW) "\n\tProject 'fclean' status: DONE\n" ${END}

# Target for rebuilding the project
re: fclean all
