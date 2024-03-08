/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:57:33 by nholbroo          #+#    #+#             */
/*   Updated: 2024/03/08 20:22:44 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

int	ft_putptr(unsigned long int n)
{
	char			str[16];
	int				i;
	int				count;

	i = 0;
	count = 0;
	if (n == 0)
		return (write(1, "(nil)", 5));
	while (n > 0)
	{
		if (n % 16 < 10)
			str[i++] = n % 16 + '0';
		else
			str[i++] = (n % 16) % 10 + 'a';
		n /= 16;
	}
	str[i++] = 'x';
	str[i++] = '0';
	str[i--] = '\0';
	while (i >= 0)
	{
		write(1, &str[i--], 1);
		count++;
	}
	return (count);
}
