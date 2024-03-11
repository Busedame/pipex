/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 20:29:25 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/11 16:57:32 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

// Standard functions. The first one appends one string to another. The second
// one compares two strings to see if they are identical. The third one
// duplicates a string.

static char	*ft_strjoin_cat_part(char const *s1, char const *s2, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		result[i] = s1[i];
		i++;
	}
	result[i++] = '/';
	while (s2[j] != '\0')
	{
		result[i] = s2[j];
		j++;
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	while (s2[j] != '\0')
		j++;
	result = (char *)malloc(i + j + 2);
	if (result == NULL)
		return (NULL);
	ft_strjoin_cat_part(s1, s2, result);
	return (result);
}

void	ft_strjoin_error(t_list *pipex, char **flags, int i)
{
	if (i == 1)
		perror("Error: Something went wrong with ft_strjoin in first child\n");
	else if (i == 2)
		perror("Error: Something went wrong with ft_strjoin in second child\n");
	ft_freearray(flags);
	free_all (pipex);
	exit(1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;

	i = 0;
	while (i < n && s1[i] == s2[i] && s1[i] != '\0'
		&& s2[i] != '\0')
		i++;
	if (i < n)
		return ((unsigned char) s1[i] - (unsigned char) s2[i]);
	return (0);
}

char	*ft_strdup(const char *s)
{
	char		*dup;
	int			i;
	size_t		len;

	i = 0;
	len = 0;
	while (s[len] != '\0')
		len++;
	dup = (char *)malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
