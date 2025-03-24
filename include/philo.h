/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:18:59 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 10:28:09 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <signal.h>
# include <stddef.h>
# include <stdlib.h>
# include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

extern int	g_debug_mode;

// gen_utils
void		log_error(const char *message);
void		enable_debug_mode(void);
void		log_output(const char *message);
int	ft_atoi(const char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
#endif