/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:46:37 by iammar            #+#    #+#             */
/*   Updated: 2025/07/09 23:29:32 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isnum(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] >= '0' && str[i] <= '9'))
			i++;
		else
			return (1);
	}
	return (0);
}

double	ft_atoi(const char *str)
{
	t_atoi		strr;
	const char	*s = str;

	strr = (t_atoi){0};
	strr.d = 1;
	strr.s = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			strr.s = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		strr.r = strr.r * 10 + (*str++ - '0');
	if (ft_isnum(s) || (strr.s * strr.r) == 0)
	{
		write(2, "invalid arguments\n", 18);
		exit(1);
	}
	return (strr.s * strr.r);
}
