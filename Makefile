# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/21 14:08:18 by nholbroo          #+#    #+#              #
#    Updated: 2024/03/08 20:45:02 by nholbroo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc
CFLAGS=-Wall -Wextra -Werror
SRC=src/ft_printf/ft_printf.c \
		src/ft_printf/ft_putchar.c \
		src/ft_printf/ft_puthexa_lower.c \
		src/ft_printf/ft_puthexa_upper.c \
		src/ft_printf/ft_putnbr.c \
		src/ft_printf/ft_putpercent.c \
		src/ft_printf/ft_putptr.c \
		src/ft_printf/ft_putstr.c \
		src/ft_printf/ft_putuint.c \
		src/ft_printf/ft_strlen.c \
		src/child_processes.c \
		src/errors.c \
		src/free_functions.c \
		src/ft_split.c \
		src/initializing.c \
		src/pipex.c \
		src/utils.c \

OBJ=$(patsubst src/%, build/%, $(SRC:.c=.o))
NAME=pipex

all:	$(NAME)

# Compiling the object files, creating an infile and an outfile, and making an executable.
$(NAME):	$(OBJ)
			@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
			@touch infile
			@touch outfile

# Making a build directory to store the object files if the build directory
# doesn't exist.
build:	
		@if [ ! -d "build" ]; \
			then mkdir -p "build"; \
		fi

# Making the subdirectories in the build-directory, and compiling the source files,
# making them object files.
build/%.o: src/%.c
		@mkdir -p $(dir $@)
		@echo "Compiling \e[1;32m$(notdir $<)\e[0m"
		@$(CC) $(CFLAGS) -Iinc -c $< -o $@;

# Cleans up all the object files, but not the build-directory or the 
# subdirectories inside.
clean:
	@rm -f build/*.o
	@rm -f build/*/*.o
	@echo "\e[1;32mAll object files removed\e[0m"

# Cleans up all object files and the build-directory, and removes the executable.
fclean:
	@rm -rf build
	@rm -f $(NAME)
	@rm infile
	@rm outfile
	@echo "\e[1;32mBuild directory, infile, outfile and executable removed\e[0m"

# Cleans up everything and compiles everything again.
re: fclean all

.PHONY: all clean fclean re
