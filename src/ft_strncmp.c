/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 20:11:47 by fvon-der          #+#    #+#             */
/*   Updated: 2025/03/24 10:28:20 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//locate character in string.
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (*s1 && *s2 && (n > 0))
	{
		if ((unsigned char)*s1 != (unsigned char)*s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
