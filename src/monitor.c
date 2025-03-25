/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:30:24 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 12:05:41 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_monitor(t_table *table)
{
	log_output("[monitor routine] started");
	pthread_mutex_lock(&table->monitor_mutex);
	pthread_cond_signal(&table->monitor_cond);
	pthread_mutex_unlock(&table->monitor_mutex);
	log_output("[monitor routine] actively checking philosopher status...");
}

int	check_philosopher_status(t_table *table, int i)
{
	long	current_time;
	long	time_since_last_meal;

	pthread_mutex_lock(&table->meal_lock);
	current_time = get_time_ms();
	time_since_last_meal = current_time - table->philos[i].last_meal;
	if (time_since_last_meal > table->t_die)
	{
		if (g_debug_mode)
		{
			printf("[monitor routine] Philosopher %d has died!\n"
				"Time since last meal: %ld ms\nThreshold: %d ms.\n",
				table->philos[i].id, time_since_last_meal, table->t_die);
			fflush(stdout);
		}
		print_status(&table->philos[i], "died");
		table->stop_simulation = 1;
		pthread_mutex_unlock(&table->meal_lock);
		pthread_mutex_lock(&table->monitor_mutex);
		pthread_cond_signal(&table->monitor_cond);
		pthread_mutex_unlock(&table->monitor_mutex);
		return (1);
	}
	pthread_mutex_unlock(&table->meal_lock);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	initialize_monitor(table);
	while (!table->stop_simulation)
	{
		i = -1;
		while (++i < table->num_philos)
			if (check_philosopher_status(table, i))
				return (NULL);
		usleep(1000);
	}
	log_output("[monitor routine] Monitor stopped gracefully.");
	return (NULL);
}
