/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 16:52:45 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/11 16:57:22 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "ft_printf.h"
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stddef.h>
# include <errno.h>

typedef struct s_list
{
	int		fd[2];
	char	**path_names;
	char	**argv;
	int		argc;
	char	*cmd1;
	char	*cmd2;
}	t_list;

// Errors
void	print_error_parent(t_list *pipex, int error_code);
void	print_error_first_child(t_list *pipex, int error_code);
void	print_error_second_child(t_list *pipex, int error_code);
void	execution_error(char **flags, char *path, int i);
void	error_invalid_command(t_list *pipex, char **flags, char *path, int i);
void	ft_strjoin_error(t_list *pipex, char **flags, int i);

// Standard functions
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);

// Free-functions
void	free_all(t_list *pipex);
int		ft_freearray(char **arr);
void	*ft_free(void *ptr);

// Initializing
int		check_file_exist(char *argv[]);
char	**find_path(t_list *pipex, char *envp[]);
t_list	*struct_init(t_list *pipex, char *envp[], char *argv[], int argc);
char	**initialize_argv(t_list *pipex, char *argv[]);
void	initialize_commands(t_list *pipex, char **command, int cmd);

// Child-processes (redirection and execution)
void	first_child_process(t_list *pipex, char *envp[], pid_t pid);
void	second_child_process(t_list *pipex, char *envp[], pid_t pid);
void	executing_first_child_process(t_list *pipex, char *envp[]);
void	executing_second_child_process(t_list *pipex, char *envp[]);

#endif