/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:17:03 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/08 20:20:52 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// Different functions to free dynamically allocated memory.

void	free_all(t_list *pipex)
{
	if (pipex->path_names)
		ft_freearray(pipex->path_names);
	if (pipex->argv)
		ft_freearray(pipex->argv);
	if (pipex->cmd1)
		ft_free(pipex->cmd1);
	if (pipex->cmd2)
		ft_free(pipex->cmd2);
	if (pipex)
		ft_free(pipex);
}

int	ft_freearray(char **arr)
{
	int	crstr;
	int	max;

	crstr = 0;
	max = 0;
	while (arr[crstr])
	{
		crstr++;
		max++;
	}
	crstr = 0;
	while (crstr < max)
	{
		free(arr[crstr]);
		arr[crstr] = NULL;
		crstr++;
	}
	free(arr);
	arr = NULL;
	return (0);
}

void	*ft_free(void *ptr)
{
	free(ptr);
	ptr = NULL;
	return (NULL);
}
