/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:35:12 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 15:46:25 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"



static int	alloc_resources(t_table *table)
{
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	table->philos = malloc(sizeof(t_philosopher)
			* table->num_philos);
	if (!table->forks || !table->philos)
	{
		log_error("Memory allocation failed.");
		free(table->forks);
		free(table->philos);
		return (-1);
	}
	log_output("Memory allocation successful.");
	return (0);
}

static int	init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
	{
		log_error("Failed to initialize print mutex.");
		return (-1);
	}
	if (pthread_mutex_init(&table->meal_lock, NULL) != 0)
	{
		log_error("Failed to initialize meal mutex.");
		return (-1);
	}
	log_output("Mutexes initialized.");
	return (0);
}

int	init_table(t_table *table, int argc, char **argv)
{
	if (!table)
	{
		log_error("NULL table pointer.");
		return (-1);
	}
	table->num_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->meals_required = -1;
	if (argc == 6)
		table->meals_required = ft_atoi(argv[5]);
	log_output("Table parameters initialized.");
    if (g_debug_mode)
        printf(("Table parameters initialized: num_philos=%d, time_to_die=%d, time_to_eat=%d, time_to_sleep=%d, meals_required=%d\n",
                 table->num_philos, table->time_to_die, table->time_to_eat, table->time_to_sleep, table->meals_required);
    if (table->num_philos <= 0 || table->time_to_die <= 0 || table->time_to_eat <= 0 || table->time_to_sleep <= 0)
    {
        log_error("[init table]: Invalid argument values.");
        return (EXIT_FAILURE);
    }
	if (alloc_resources(table) == -1 || init_mutexes(table) == -1
		|| init_philosophers(table) == -1)
		return (EXIT_FAILURE);
	return (0);
}


void	free_table(t_table *table)
{
	int	i;

	if (!table)
	{
		log_error("[free_table]: Attempted to free a NULL table.");
		return ;
	}
	if (table->forks)
	{
		i = 0;
		while (i < table->num_philos)
		{
			if (pthread_mutex_destroy(&table->forks[i]) != 0)
				log_error("[free_table]: Failed to destroy a fork mutex.");
			i++;
		}
		free(table->forks);
	}
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->meal_lock);
	if (table->philos)
		free(table->philos);

	log_output("[free_table]: Successfully freed table resources.");
}
