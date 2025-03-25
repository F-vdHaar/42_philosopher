/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:25:19 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 20:01:23 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

static void	wait_for_threads(t_table *table);


void	start_monitor(t_table *table)
{
	if (pthread_create(&table->monitor_thread, NULL,
		monitor_routine, table) != 0)
	{
		log_error("[start_monitor]: Failed to create monitor thread.");
		table->stop_simulation = 1;
		wait_for_threads(table);
		return;
	}
	log_output("Monitor started.");
}

static void wait_for_threads(t_table *table)
{
	int i;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			log_error("[wait_for_threads]: Failed to join philosopher thread.");
		i++;
	}
	log_output("Philosophers finished.");
}

void	stop_monitor(t_table *table)
{
	table->stop_simulation = 1;
	if (pthread_join(table->monitor_thread, NULL) != 0)
		log_error("[stop_monitor]: Failed to join monitor thread.");
	log_output("Monitor stopped.");
}

void    start_simulation(t_table *table)
{
	table->start_time = get_time_ms();
	table->stop_simulation = 0;
	log_output("[start simulation] Starting simulation...");
	log_output("[start simulation] Starting monitor...");
	start_monitor(table);
	pthread_mutex_lock(&table->monitor_mutex);
	log_output("[start simulation] Starting philosophers...");
	start_philosophers(table);
	log_output("[start simulation] Monitor waiting for philosophers...");
	pthread_cond_wait(&table->monitor_cond, &table->monitor_mutex);
	pthread_mutex_unlock(&table->monitor_mutex);
	log_output("[start simulation] Simulation running...");
	while (!table->stop_simulation)
		usleep(100);
	log_output("[start simulation] stop simulation flag set.");
	wait_for_threads(table);
	stop_monitor(table);
	log_output("[start simulation] Simulation ended.");
}
