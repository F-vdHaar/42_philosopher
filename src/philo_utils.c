/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:44:51 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 19:20:20 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philosopher *philo, char *status)
{
	pthread_mutex_lock(&philo->table->print_lock);
	if (!philo->table->stop_simulation)
	{
		printf("%ld %d %s\n", get_time_ms() - philo->table->start_time,
			philo->id, status);
		fflush(stdout);
	}
	pthread_mutex_unlock(&philo->table->print_lock);
}

// Helper: wait depending on hunger before retrying fork acquisition
void	wait_retry(t_philosopher *philo, long current_time)
{
	long	time_remaining;

	time_remaining = philo->table->t_die - (current_time - philo->last_meal);
	if (time_remaining > 500)
		usleep(time_remaining / 2 + 1000);
	else if (time_remaining > 100)
		usleep(time_remaining / 3 + 1000);
	else
		usleep(time_remaining / 5 + 1000);
}

int	take_forks(t_philosopher *philo)
{
	if (pthread_mutex_lock(philo->left_fork) != 0)
	{
		if (g_debug_mode)
			print_status(philo, "failed to take left fork");
		return (-1);
	}
	print_status(philo, "has taken a fork");
	if (pthread_mutex_trylock(philo->right_fork) == 0)
	{
		print_status(philo, "has taken second fork");
		return (1);
	}
	else
	{
		print_status(philo, "failed take second fork, releasing fork");
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
}

// Helper: Release both forks
void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	if (g_debug_mode)
		print_status(philo, "released forks after eating");
}
