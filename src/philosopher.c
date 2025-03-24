/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:51 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 15:40:13 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "philo.h"

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
			return (-1);
		}
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% table->num_philos];
		table->philos[i].table = table;
		i++;
	}
	log_output("Philosophers initialized.");
	return (0);
}

void	start_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].last_meal_time = table->start_time;
		if (pthread_create(&table->philos[i].thread, NULL,
			 philosopher_routine, &table->philos[i]) != 0)
		{
			log_error("[start_philosophers]: Failed to create philosopher thread.");
			table->stop_simulation = 1;
			while (--i >= 0)
				pthread_join(table->philos[i].thread, NULL);
			return;
		}
		i++;
	}
	log_output("Philosophers started.");
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;

    while (!philo->table->stop_simulation)
    {
        print_status(philo, "is thinking");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");

        if (pthread_mutex_trylock(philo->right_fork) == 0)
        {
            print_status(philo, "has taken a fork");
            print_status(philo, "is eating");
            pthread_mutex_lock(&philo->table->meal_lock);
            philo->last_meal_time = get_time_ms();
            philo->meals_eaten++;
            pthread_mutex_unlock(&philo->table->meal_lock);
            usleep(philo->table->time_to_eat * 1000);
            pthread_mutex_unlock(philo->right_fork);
            pthread_mutex_unlock(philo->left_fork);
            if (philo->table->meals_required > 0 &&
                philo->meals_eaten >= philo->table->meals_required)
                return NULL;
            print_status(philo, "is sleeping");
            usleep(philo->table->time_to_sleep * 1000);
        }
        else
        {
            print_status(philo, "releases fork");
            pthread_mutex_unlock(philo->left_fork);
            usleep(100);  // TODO (optional) make this dependen on hunger level
        }
    }
    return NULL;
}
