NAME = horde

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

OBJ_DIR = obj

SRCS = Zombie.cpp ZombieHorde.cpp main.cpp
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME) built"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(NAME): objects cleaned"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME): executable cleaned"

re: fclean all

.PHONY: all clean fclean re
