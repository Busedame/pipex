/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa_upper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:34:52 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/08 20:22:38 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

int	ft_puthexa_upper(unsigned int n)
{
	char	str[9];
	int		i;
	int		count;

	i = 0;
	count = 0;
	if (n == 0)
		return (write(1, "0", 1));
	while (n > 0)
	{
		if (n % 16 < 10)
			str[i] = n % 16 + '0';
		else
			str[i] = (n % 16) % 10 + 'A';
		n /= 16;
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
