/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:30:24 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 18:01:08 by fvon-de          ###   ########.fr       */
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

int	has_philosopher_died(t_table *table, int i)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time_ms();
	time_since_last_meal = current_time - table->philos[i].last_meal;
	if (time_since_last_meal > table->t_die)
	{
		if (g_debug_mode)
			printf("[monitor-sub] Philo %d died! Time: %ld ms (Limit: %d ms)\n",
				table->philos[i].id, time_since_last_meal, table->t_die);
		print_status(&table->philos[i], "died");
		table->stop_simulation = 1;
		return (1);
	}
	return (0);
}

int	has_everyone_eaten_enough(t_table *table)
{
	int	j;

	j = 0;
	while (j < table->num_philos)
	{
		if (table->philos[j].meals_eaten < table->meals_required)
			return (0);
		j++;
	}
	printf("All philosophers have eaten at least %d times.\n",
		table->meals_required);
	fflush(stdout);
	table->stop_simulation = 1;
	return (1);
}

int	check_philosopher_status(t_table *table, int i)
{
	int	simulation_end;

	simulation_end = 0;
	pthread_mutex_lock(&table->meal_lock);
	if (has_philosopher_died(table, i)
		|| (table->meals_required > 0 && has_everyone_eaten_enough(table)))
	{
		log_output("[check_philosopher_status]: died or eaten enough");
		log_output("[check_philosopher_status]: simulation stop flag set");
		simulation_end = 1;
	}
	pthread_mutex_unlock(&table->meal_lock);
	if (simulation_end)
	{
		pthread_mutex_lock(&table->monitor_mutex);
		pthread_cond_signal(&table->monitor_cond);
		pthread_mutex_unlock(&table->monitor_mutex);
	}
	return (simulation_end);
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
		{
			if (check_philosopher_status(table, i))
			{
				log_output("[monitor routine] retrun due philo status");
				return (NULL);
			}
		}
		usleep(1000);
	}
	log_output("[monitor routine] TODO : check if this is reached");
	return (NULL);
}
