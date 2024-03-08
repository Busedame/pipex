/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 19:03:17 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/08 20:29:13 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// There are going to be two child processes going on at the same time. Once
// they are both finished, the parent process will make sure to free allocated
// memory. For the child processes the freeing is happening within designated
// functions.

int	main(int argc, char *argv[], char *envp[])
{
	t_list	*pipex;
	pid_t	pid;

	pipex = NULL;
	pid = 0;
	if (argc != 5)
	{
		errno = EINVAL;
		perror("Error: Need four arguments");
		return (1);
	}
	if (check_file_exist(argv) == -1)
		return (1);
	pipex = struct_init(pipex, envp, argv, argc);
	first_child_process(pipex, envp, pid);
	second_child_process(pipex, envp, pid);
	free_all(pipex);
	return (0);
}
