# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/14 16:25:06 by etetopat          #+#    #+#              #
#    Updated: 2023/12/27 19:45:02 by jgoldste         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ***** EXECUTABLE ***** #
NAME = webserv

# ***** COLORS ********* #
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
BLUE	= \033[0;34m
MAGENTA	= \033[0;35m
CYAN	= \033[0;36m
BOLD	= \033[1m
ITALIC	= \033[3m
ULINE	= \033[4m
BLINK	= \033[5m
NC		= \033[0m

# ***** COMPILATION **** #
CXX			= g++ -g3
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
INC			= -I
RM			= rm -rf

# ***** DIRECTORIES ******** #
HDR_DIR	= ./inc
SRC_DIR	= ./src
OBJ_DIR	= ./obj

# ***** SERVER ***** #
SERV_DIR	= /serv
SERV_HDRS	=
SERV_SRCS	=

# ***** REQUEST & RESPONSE ***** #
REQ_RESP_DIR	= /req_resp
REQ_RESP_HDRS	=
REQ_RESP_SRCS	=

# ***** CONFIG & CGI ***** #
CONFIG_DIR	= /config_cgi
CONFIG_HDRS	=
CONFIG_SRCS	=

MKDIR	= mkdir -p
DSYM	= $(wildcard *.dSYM)

.PHONY : all clean fclean re mem1 mem2 mem3

# # ***** HEADERS ******** #
# HDR = ./inc/Webserv.hpp

# # ***** SOURCES ******** #
# SRC_DIR = ./src/
# SRC = main.cpp Client.cpp Logger.cpp ServerManager.cpp Utils.cpp Request.cpp
# SRCS = $(addprefix $(SRC_DIR), $(SRC))

# # ***** OBJECTS ******** #
# OBJ_DIR = ./obj/
# OBJ = $(SRC:.cpp=.o)
# OBJS = $(addprefix $(OBJ_DIR), $(OBJ))

# ***** RULES ********** #
all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)
	$(MKDIR) $(OBJ_DIR)/
	$(MKDIR) $(OBJ_DIR)/
	$(MKDIR) $(OBJ_DIR)/

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HDR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@echo "\n$(GREEN)---- (っ▀¯▀)つ $(NAME) is ready! ----$(NC)\n"

clean:
	@$(RM) $(OBJ_DIR) $(DSYM)
	@echo "\n$(YELLOW) $(RM) $(OBJ_DIR) $(NC)"
	@echo "\n$(RED)---- ᕦ(ò_óˇ)ᕤ $(NAME) is clean! ----$(NC)\n"

fclean: clean
	@$(RM) $(NAME)
	@echo "\n$(YELLOW) $(RM) $(NAME) $(NC)"
	@echo "\n$(RED)---- (╯°□°）╯︵ ┻━┻ $(NAME) is gone! ----$(NC)\n"

re: fclean all

# ***** MEMORY LEAKS **** #
mem1:
	valgrind --leak-check=summary ./$(NAME)
mem2:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)
mem3:
	valgrind -s ./$(NAME)

# $(OBJ_DIR)%.o: $(SRC_DIR)%.cpp $(HDR)
# 	# @echo "$(MAGENTA) $(CXX) $(CXXFLAGS) $(HDR) -c $< -o $@ $(NC)"
# 	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# $(NAME): $(OBJS)
# 	@echo "\n$(BLUE) $(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(NC)"
# 	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
# 	@echo "\n$(GREEN)---- (っ▀¯▀)つ $(NAME) is ready! ----$(NC)\n"

# clean:
# 	@$(RM) $(OBJ_DIR) $(DSYM)
# 	@echo "\n$(YELLOW) $(RM) $(OBJ_DIR) $(NC)"
# 	@echo "\n$(RED)---- ᕦ(ò_óˇ)ᕤ $(NAME) is clean! ----$(NC)\n"

# fclean: clean
# 	@$(RM) $(NAME)
# 	@echo "\n$(YELLOW) $(RM) $(NAME) $(NC)"
# 	@echo "\n$(RED)---- (╯°□°）╯︵ ┻━┻ $(NAME) is gone! ----$(NC)\n"

# re: fclean all

# # ***** MEMORY LEAKS **** #
# mem1:
# 	valgrind --leak-check=summary ./$(NAME)
# mem2:
# 	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)
# mem3:
# 	valgrind -s ./$(NAME)
