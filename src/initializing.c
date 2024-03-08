/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:51:17 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/08 20:27:49 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// Initializes the struct, and each part of the struct. Also a function that
// checks whether the files input exist or not.

t_list	*struct_init(t_list *pipex, char *envp[], char *argv[], int argc)
{
	pipex = malloc(sizeof(t_list));
	if (!pipex)
		print_error_parent(pipex, 1);
	pipex->argc = argc;
	pipex->path_names = find_path(pipex, envp);
	if (!pipex->path_names)
		print_error_parent(pipex, 2);
	if (pipe(pipex->fd) == -1)
		print_error_parent(pipex, 3);
	pipex->argv = initialize_argv(pipex, argv);
	if (!pipex->argv)
		print_error_parent(pipex, 4);
	pipex->cmd1 = NULL;
	pipex->cmd2 = NULL;
	return (pipex);
}

int	check_file_exist(char *argv[])
{
	if (access(argv[1], F_OK) == -1 || access(argv[4], F_OK == -1))
	{
		if (access(argv[1], F_OK) == -1)
			perror("Error: First file does not exist");
		if (access(argv[4], F_OK) == -1)
		{
			open(argv[4], O_CREAT);
			perror("Second file does not exist (File created)");
		}
		return (-1);
	}
	return (0);
}

char	**find_path(t_list *pipex, char *envp[])
{
	int		str;
	char	*path_line;

	str = 0;
	path_line = NULL;
	while (envp[str])
	{
		if ((ft_strncmp(envp[str], "PATH=", 5) == 0))
		{
			path_line = ft_strdup(envp[str]);
			if (!path_line)
				return (NULL);
			break ;
		}
		str++;
	}
	if (path_line)
	{
		pipex->path_names = ft_split(&path_line[5], ':');
		free(path_line);
	}
	return (pipex->path_names);
}

void	initialize_commands(t_list *pipex, char **command, int nbr)
{
	if (nbr == 1)
	{
		pipex->cmd1 = ft_strdup(*command);
		if (!pipex->cmd1)
			print_error_first_child(pipex, 6);
	}
	else if (nbr == 2)
	{
		pipex->cmd2 = ft_strdup(*command);
		if (!pipex->cmd2)
			print_error_second_child(pipex, 7);
	}
}

char	**initialize_argv(t_list *pipex, char *argv[])
{
	int	str;

	str = 0;
	pipex->argv = malloc(sizeof(char *) * (pipex->argc + 1));
	if (!pipex->argv)
		return (NULL);
	while (argv[str])
	{
		pipex->argv[str] = ft_strdup(argv[str]);
		if (!pipex->argv[str])
			return (NULL);
		str++;
	}
	pipex->argv[str] = NULL;
	return (pipex->argv);
}
