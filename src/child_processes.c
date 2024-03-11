/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 14:46:27 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/11 18:38:05 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// The redirection and execution of commands. The forking is happening
// in the two bottom functions, and the child processes are happening
// simultaneously - while the parent process is waiting in both cases.
// The two functions in the bottom take care of the redirection of input
// and output. The two functions on top take care of finding the correct
// path to the command that is going to be executed, and in the end executes
// the command.

void	executing_second_child_process(t_list *pipex, char *envp[])
{
	char	*path;
	int		i;
	char	**flags;

	path = NULL;
	i = 0;
	flags = ft_split(pipex->argv[3], ' ');
	if (!flags)
		print_error_second_child(pipex, 6);
	initialize_commands(pipex, &flags[0], 2);
	while (pipex->path_names[i])
	{
		if (path)
			ft_free(path);
		path = ft_strjoin(pipex->path_names[i++], pipex->cmd2);
		if (!path)
			ft_strjoin_error(pipex, flags, 1);
		if (access(path, X_OK) == 0)
			break ;
	}
	if (access(path, X_OK) == -1)
		error_invalid_command(pipex, flags, path, 2);
	free_all(pipex);
	execve(path, flags, envp);
	execution_error(flags, path, 2);
}

void	executing_first_child_process(t_list *pipex, char *envp[])
{
	char	*path;
	int		i;
	char	**flags;

	path = NULL;
	i = 0;
	flags = ft_split(pipex->argv[2], ' ');
	if (!flags)
		print_error_first_child(pipex, 6);
	initialize_commands(pipex, &flags[0], 1);
	while (pipex->path_names[i])
	{
		if (path)
			ft_free(path);
		path = ft_strjoin(pipex->path_names[i++], pipex->cmd1);
		if (!path)
			ft_strjoin_error(pipex, flags, 1);
		if (access(path, X_OK) == 0)
			break ;
	}
	if (access(path, X_OK) == -1)
		error_invalid_command(pipex, flags, path, 1);
	free_all(pipex);
	execve(path, flags, envp);
	execution_error(flags, path, 1);
}

void	first_child_process(t_list *pipex, char *envp[], pid_t pid)
{
	int		input;

	pid = fork();
	if (pid < 0)
		print_error_first_child(pipex, 1);
	if (!pid)
	{
		input = open(pipex->argv[1], O_RDONLY);
		if (input == -1)
			print_error_first_child(pipex, 2);
		if (dup2(input, STDIN_FILENO) == -1)
			print_error_first_child(pipex, 3);
		if (dup2(pipex->fd[1], STDOUT_FILENO) == -1)
			print_error_first_child(pipex, 4);
		if (close(pipex->fd[0]) == -1)
			print_error_first_child(pipex, 5);
		executing_first_child_process(pipex, envp);
	}
	else
	{
		if (close(pipex->fd[1]) == -1)
			print_error_parent(pipex, 5);
		if (waitpid(pid, NULL, 0) == -1)
			print_error_parent(pipex, 6);
	}
}

void	second_child_process(t_list *pipex, char *envp[], pid_t pid)
{
	int		output;

	pid = fork();
	if (pid < 0)
		print_error_second_child(pipex, 1);
	if (!pid)
	{
		output = open(pipex->argv[4], O_WRONLY | O_TRUNC);
		if (output == -1)
			print_error_second_child(pipex, 2);
		if (dup2(pipex->fd[0], STDIN_FILENO) == -1)
			print_error_second_child(pipex, 3);
		if (dup2(output, STDOUT_FILENO) == -1)
			print_error_second_child(pipex, 4);
		if (close(pipex->fd[1]) == -1)
			print_error_second_child(pipex, 5);
		executing_second_child_process(pipex, envp);
	}
	else
	{
		if (close(pipex->fd[0]) == -1)
			print_error_parent(pipex, 5);
		if (waitpid(pid, NULL, 0) == -1)
			print_error_parent(pipex, 6);
	}
}
