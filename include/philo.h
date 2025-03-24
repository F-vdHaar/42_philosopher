/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:18:59 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 10:46:11 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <signal.h>
# include <stddef.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philosopher;

struct s_table
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_required;
	int				stop_simulation;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_lock;
	t_philosopher	*philos;
};

extern int	g_debug_mode;

// Table
void	free_table(t_table *table);
int	init_table(t_table *table, int argc, char **argv);



// gen_utils
void		log_error(const char *message);
void		enable_debug_mode(void);
void		log_output(const char *message);

int         ft_atoi(const char *str);
int         ft_strncmp(const char *s1, const char *s2, size_t n);
#endif