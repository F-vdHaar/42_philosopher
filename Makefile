# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/18 20:24:03 by fvon-de           #+#    #+#              #
#    Updated: 2025/03/24 10:15:30 by fvon-de          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COLOR_RESET     = \033[0m
COLOR_INFO      = \033[36m  # Info messages (light cyan)
COLOR_WARN      = \033[33m  # Warnings or compiling steps (yellow)
COLOR_SUCCESS   = \033[32m  # Success messages (green)
COLOR_ERROR     = \033[31m  # Error messages (red)
COLOR_HEADER    = \033[1;34m # Header or category titles (blue)

# Project settings
NAME            = philo
NAME_DEBUG      = philo_debug
INCLUDE_DIR     = include
SRC_DIR         = src
OBJ_DIR         = obj

# Compiler settings
CC              = cc
CFLAGS          = -Wall -Wextra -Werror -Wunused -I$(INCLUDE_DIR)
DEBUG_FLAGS     = -I$(INCLUDE_DIR) \

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# For Debug
$(info Sources: $(SRCS))
$(info Objects: $(OBJS))

# Default rule
all:  $(NAME)
	@echo "$(COLOR_SUCCESS)Build complete: $(NAME)$(COLOR_RESET)"

# Ensure object directory exists before compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	@echo "$(COLOR_WARN)Compiling $<$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

# Ensure object directory for debug exists before compiling
$(OBJ_DIR)/debug/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/debug/$(dir $*)
	@echo "$(COLOR_WARN)Compiling $< with DEBUG_FLAGS$(COLOR_RESET)"
	@$(CC) $(DEBUG_FLAGS) -c $< -o $@

# Ensure main object directory exists
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(COLOR_WARN)Created directory: $(OBJ_DIR)$(COLOR_RESET)"


# Build project
$(NAME): $(OBJS) 
	@echo "$(COLOR_WARN)Linking $(NAME)...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 
	@echo "$(COLOR_SUCCESS)Linking successful: $(NAME)$(COLOR_RESET)"

# Build debug version
$(NAME_DEBUG): $(OBJS:$(OBJ_DIR)/%.o=$(OBJ_DIR)/debug/%.o) 
	@echo "$(COLOR_HEADER)Linking $(NAME_DEBUG)...$(COLOR_RESET)"
	$(CC) $(DEBUG_FLAGS) $(OBJS:$(OBJ_DIR)/%.o=$(OBJ_DIR)/debug/%.o) -o $(NAME_DEBUG) 
	@echo "$(COLOR_SUCCESS)$(NAME_DEBUG) built successfully$(COLOR_RESET)"

# Cleaning rules
clean:
	@echo "$(COLOR_HEADER)Cleaning object files...$(COLOR_RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(COLOR_SUCCESS)Object files cleaned$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_HEADER)Removing executables...$(COLOR_RESET)"
	@rm -f $(NAME) $(NAME_DEBUG)
	@echo "$(COLOR_SUCCESS)All cleaned up$(COLOR_RESET)"

re: fclean all

# Debug rules
debug:  $(NAME_DEBUG)
	@echo "$(COLOR_ERROR)Compiling in debug mode...$(COLOR_RESET)"

redebug: fclean debug

# Norminette check
norm:
	@echo "$(COLOR_HEADER)Running Norminette...$(COLOR_RESET)"
	@-norminette $(SRC_DIR) | grep -v "OK!" || true
	@-norminette $(INCLUDE_DIR) | grep -v "OK!" || true
	@echo "$(COLOR_SUCCESS)Norminette check complete$(COLOR_RESET)"

.PHONY: all clean fclean re norm debug redebug
