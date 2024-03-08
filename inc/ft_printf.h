/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nholbroo <nholbroo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:53:26 by nholbroo          #+#    #+#             */
/*   Updated: 2023/12/07 19:33:18 by nholbroo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>
# include <stddef.h>

int		ft_printf(const char *txt, ...);

int		ft_putnbr(int n);

int		ft_putuint(unsigned int n);

int		ft_puthexa_upper(unsigned int n);

int		ft_puthexa_lower(unsigned int n);

int		ft_putptr(unsigned long int n);

int		ft_putchar(char c);

int		ft_putstr(char *s);

size_t	ft_strlen(const char *s);

int		ft_putpercent(void);

#endif
