/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:30:24 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 22:01:35 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void *monitor_routine(void *arg)
{
    t_table *table = (t_table *)arg;

    log_output("[DEBUG] [monitor routine] started");

    // Signal that the monitor is ready to begin
    pthread_mutex_lock(&table->monitor_mutex);
    pthread_cond_signal(&table->monitor_cond);
    pthread_mutex_unlock(&table->monitor_mutex);

    log_output("[DEBUG] Monitor is actively checking for philosopher status...");

    // Monitoring loop
    while (!table->stop_simulation)
    {
        int i = 0;
        while (i < table->num_philos)
        {
            pthread_mutex_lock(&table->meal_lock);

            // Checking if the philosopher has exceeded the time limit and should be considered "dead"
            long current_time = get_time_ms();
            long time_since_last_meal = current_time - table->philos[i].last_meal_time;
            
            if (time_since_last_meal > table->time_to_die)
            {
                // Enhanced logging when a philosopher dies
                if (g_debug_mode)
                {
                    printf("[ERROR] Philosopher %d has died! Last meal time was %ld ms, current time is %ld ms, time since last meal = %ld ms.\n",
                        i, table->philos[i].last_meal_time, current_time, time_since_last_meal);
                }

                // Print status of the philosopher that has died
                print_status(&table->philos[i], "died");

                // Log the death event with detailed context
                printf("[ERROR] Philosopher %i has died at %ld ms! Time since last meal: %ld ms (Time to Die Threshold: %i ms).",
                    table->philos[i].id, current_time, time_since_last_meal, table->time_to_die);

                // Set the simulation to stop
                table->stop_simulation = 1;

                pthread_mutex_unlock(&table->meal_lock);

                // Signal to stop everything
                pthread_mutex_lock(&table->monitor_mutex);
                pthread_cond_signal(&table->monitor_cond);
                pthread_mutex_unlock(&table->monitor_mutex);

                printf("[DEBUG] Monitor detected a fatal event: Philosopher %d died. Stopping simulation...", table->philos[i].id);
                return NULL;
            }

            pthread_mutex_unlock(&table->meal_lock);
            i++;
        }
    }

    log_output("[DEBUG] Monitor stopped gracefully.");
    return NULL;
}
