/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:51 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 22:45:29 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Left fork = own index
// Right fork = next index
int	init_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			log_error("Failed to initialize fork mutex.");
			return (EXIT_FAILURE);
		}
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% table->num_philos];
		table->philos[i].table = table;
		table->philos[i].last_meal = get_time_ms();
		i++;
	}
	log_output("[init]: Philosophers initialized.");
	return (EXIT_SUCCESS);
}

void	start_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].last_meal = get_time_ms();
		if (pthread_create(&table->philos[i].thread,
				NULL, philos_routine, &table->philos[i]) != 0)
		{
			log_error("[ERROR] Failed to create philosopher thread %d\n");
			while (i > 0)
			{
				i--;
				pthread_cancel(table->philos[i].thread);
				pthread_join(table->philos[i].thread, NULL);
			}
			return ;
		}
		if (g_debug_mode)
			printf("[start_philo] ID %i  T_l_meal: %ld ms (Thresh: %i ms).\n",
				table->philos[i].id, table->philos[i].last_meal,
				table->philos[i].table->t_die);
		i++;
	}
	pthread_cond_broadcast(&table->monitor_cond);
}

void	philo_eat_and_sleep(t_philosopher *philo)
{
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->table->meal_lock);
	philo->last_meal = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->meal_lock);
	usleep(philo->table->time_to_eat * 1000);
	release_forks(philo);
	if (philo->table->meals_required > 0
		&& philo->meals_eaten >= philo->table->meals_required)
		pthread_exit(NULL);
	if (g_debug_mode)
	{
		printf("[philo routine] P %i  T_l_meal: %ld Thresh: %i).\n",
			philo->id, philo->last_meal, philo->table->t_die);
		fflush(stdout);
	}
	print_status(philo, "is sleeping");
	usleep(philo->table->time_to_sleep * 1000);
	print_status(philo, "is thinking");
}

void	*philos_routine(void *arg)
{
	t_philosopher	*philo;
	long			current_time;

	philo = (t_philosopher *)arg;
	if (!philo || !philo->left_fork || !philo->right_fork)
		return (log_error("Philosopher is NULL or has invalid forks!"), NULL);
	if (philo->id % 2 == 0)
		usleep(500);
	while (!philo->table->stop_simulation)
	{
		current_time = get_time_ms();
		if (take_forks(philo))
		{
			philo_eat_and_sleep(philo);
			release_forks(philo);
		}
		else
			wait_retry(philo, current_time);
	}
	return (NULL);
}
