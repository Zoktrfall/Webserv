NAME := webserv
CPP := c++
CPPFLAGS := -std=c++98 -Wall -Wextra -Werror
RM := rm -rf
OUTPUT := ./${NAME}

SRC_DIR := src
OBJ_DIR := obj

SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

SRC_SUB_DIRS := $(shell find $(SRC_DIR) -type d)
OBJ_SUB_DIRS := $(foreach dir, $(SRC_SUB_DIRS), $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR)/, ,$(dir))))

LIB_DIRS := $(shell find $(SRC_DIR) -type f -name '*.hpp')
LIBS := $(foreach libdir, $(LIB_DIRS), $(addprefix -I, $(dir $(libdir))))

RESET = \033[0m
RED = \033[91m
YELLOW = \033[33m
BLUE = \033[94m
GREEN = \033[36m

all : $(NAME)

$(NAME): Makefile $(OBJS)
	@echo "${GREEN}Welcome to Miracle${RESET}"
	@$(CPP) -g $(CPPFLAGS) $(LIBS) -o $(NAME) $(OBJS)
	@echo "${GREEN}Build successful.${RESET}"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@printf "${BLUE}Compiling $<...${RESET}\n"
	@mkdir -p $(OBJ_DIR) $(OBJ_SUB_DIRS)
	@$(CPP) $(CPPFLAGS) $(LIBS) -c $< -o $@

clean :
	@echo "${YELLOW}Deleting object files...${RESET}"
	@$(RM) $(OBJ_DIR)

fclean : clean
	@echo "${RED}Deleting $(NAME)...${RESET}"
	@$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
