/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:30:24 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 11:31:06 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!table->stop_simulation)
	{
		i = 0;
		while (i < table->num_philos)
		{
			pthread_mutex_lock(&table->meal_lock);
			if (get_time_ms() - table->philos[i].last_meal_time >
				table->time_to_die)
			{
				print_status(&table->philos[i], "died");
				table->stop_simulation = 1;
			}
			pthread_mutex_unlock(&table->meal_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}