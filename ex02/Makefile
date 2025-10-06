# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: miyuu <miyuu@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/31 18:25:47 by miyuu             #+#    #+#              #
#    Updated: 2025/08/04 12:19:48 by miyuu            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe
SRCS = \
		main.cpp \
		PmergeMe.cpp

HEADERS = \
		PmergeMe.hpp \

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

.PHONY: all clean fclean re

all: $(NAME)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# ========== File Dependency ========== #

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
