/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:44:51 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 11:46:03 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

void	print_status(t_philosopher *philo, char *status)
{
	pthread_mutex_lock(&philo->table->print_lock);
	if (!philo->table->stop_simulation)
		printf("%ld %d %s\n", get_time_ms() - philo->table->start_time,
			philo->id, status);
	pthread_mutex_unlock(&philo->table->print_lock);
}

/* Utility to get current timestamp in milliseconds */
long get_time_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}