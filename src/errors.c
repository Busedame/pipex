/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 15:51:57 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/11 16:58:33 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// Prints all types of error messages, specified with error codes. Also takes
// care of freeing allocated memory and exiting the child process in the case
// of an error.

void	print_error_first_child(t_list *pipex, int error_code)
{
	if (error_code == 0)
		perror("Error: Could not allocate memory for command 1\n");
	else if (error_code == 1)
		perror("Error: Fork for first child failed\n");
	else if (error_code == 2)
		perror("Error: Could not open first file\n");
	else if (error_code == 3)
		perror("Error: Could not redirect input in first child\n");
	else if (error_code == 4)
		perror("Error: Could not redirect output in first child\n");
	else if (error_code == 5)
		perror("Error: Could not close other side of pipe in first child\n");
	else if (error_code == 6)
		perror("Error: Something went wrong with ft_split in first child\n");
	free_all(pipex);
	exit(1);
}

void	print_error_second_child(t_list *pipex, int error_code)
{
	if (error_code == 0)
		perror("Error: Could not allocate memory for command 2\n");
	else if (error_code == 1)
		perror("Error: Fork for second child failed\n");
	else if (error_code == 2)
		perror("Error: Could not open second file\n");
	else if (error_code == 3)
		perror("Error: Could not redirect input in second child\n");
	else if (error_code == 4)
		perror("Error: Could not redirect output in second child\n");
	else if (error_code == 5)
		perror("Error: Could not close other side of pipe in second child\n");
	else if (error_code == 6)
		perror("Error: Something went wrong with ft_split in second child\n");
	free_all(pipex);
	exit(1);
}

void	print_error_parent(t_list *pipex, int error_code)
{
	if (error_code == 1)
	{
		perror("Error: Could not allocate struct\n");
		exit(1);
	}
	else if (error_code == 2)
	{
		if (errno == EINVAL)
			perror("Error: Could not find PATH\n");
		else if (errno == ENOMEM)
			perror("Error: Could not allocate command paths\n");
	}
	else if (error_code == 3)
		perror("Error: Pipe function failed\n");
	else if (error_code == 4)
		perror("Error: Could not allocate arguments\n");
	else if (error_code == 5)
		perror("Error: Could not close other side of pipe in parent\n");
	else if (error_code == 6)
		perror("Waitpid function failed\n");
	free_all(pipex);
	exit(1);
}

void	error_invalid_command(t_list *pipex, char **flags, char *path, int i)
{
	if (i == 1)
		perror("Error: Command 1 is not valid\n");
	else if (i == 2)
		perror("Error: Command 2 is not valid\n");
	ft_freearray(flags);
	free(path);
	free_all (pipex);
	exit(1);
}

void	execution_error(char **flags, char *path, int i)
{
	if (i == 1)
		perror("Execution of first command failed\n");
	else if (i == 2)
		perror("Execution of second command failed\n");
	ft_freearray(flags);
	free(path);
	exit(1);
}
