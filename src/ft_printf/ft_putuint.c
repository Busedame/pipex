/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putuint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 17:25:21 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/08 20:22:49 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

static int	ft_nbwrite(char *str, unsigned int n)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (n > 0)
	{
		str[i] = n % 10 + '0';
		n /= 10;
		i++;
	}
	str[i--] = '\0';
	while (i >= 0)
	{
		write(1, &str[i--], 1);
		count++;
	}
	return (count);
}

int	ft_putuint(unsigned int n)
{
	char	str[11];
	int		i;

	i = 0;
	if (n == 0)
		return (write(1, "0", 1));
	return (ft_nbwrite(str, n) + i);
}
