/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:29:51 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 09:39:08 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "philo.h"

// Left fork = own index
// Right fork = next index
int init_philosophers(t_table *table)
{
	int i;

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
        table->philos[i].last_meal_time = get_time_ms();; 
        printf("[init] Philosopher %i  Time since last meal: %ld ms (Time to Die Threshold: %i ms).\n",
                table->philos[i].id, table->philos[i].last_meal_time, table->philos[i].table->time_to_die);
		fflush(stdout);
                i++;
	}
	log_output("[init]: Philosophers initialized.");
	return (0);
}
void start_philosophers(t_table *table)
{
    int i;

    i = 0;
    while (i < table->num_philos)
    {
        table->philos[i].last_meal_time = get_time_ms();
        if (pthread_create(&table->philos[i].thread, NULL, philosopher_routine, &table->philos[i]) != 0)
        {
            if (g_debug_mode)
            printf("[ERROR] Failed to create philosopher thread %d\n", i);
            while (i > 0)
            {
                i--;
                pthread_cancel(table->philos[i].thread);
                pthread_join(table->philos[i].thread, NULL);
            }
            return;
        }
        if (g_debug_mode)
            printf("[DEBUG] Philosopher %d initialized at %ld ms\n", i, table->philos[i].last_meal_time);
         printf("[routine] Philosopher %i  Time since last meal: %ld ms (Time to Die Threshold: %i ms).",
            table->philos[i].id, table->philos[i].last_meal_time, table->philos[i].table->time_to_die);
            i++;
    }
    pthread_cond_broadcast(&table->monitor_cond);
    log_output("[DEBUG] All philosophers started.");
}

int take_forks(t_philosopher *philo) {
    if (pthread_mutex_lock(philo->left_fork) != 0) {
        log_error("Error locking left fork");
        return -1;
    }
    print_status(philo, "has taken a left fork");

    // Try to take the right fork
    if (pthread_mutex_trylock(philo->right_fork) == 0) {
        print_status(philo, "has taken a right fork");
        return 1;
    } else {
        // Failed to take the right fork, release the left
        pthread_mutex_unlock(philo->left_fork);
        print_status(philo, "couldn't take right fork, releasing left fork");
        return 0;
    }
}

// Helper: Release both forks
void release_forks(t_philosopher *philo) {
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
    print_status(philo, "released forks after eating");
}


void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    long current_time;

    if (!philo || !philo->left_fork || !philo->right_fork) {
        log_error("Philosopher is NULL or has invalid forks!");
        return NULL;
    }
    while (!philo->table->stop_simulation)
    {
        current_time = get_time_ms();
        if (take_forks(philo))
        {
            print_status(philo, "is eating");
            pthread_mutex_lock(&philo->table->meal_lock);
            philo->last_meal_time = get_time_ms();;
            philo->meals_eaten++;
            pthread_mutex_unlock(&philo->table->meal_lock);
            usleep(philo->table->time_to_eat * 1000);
            pthread_mutex_lock(&philo->table->meal_lock);
            philo->last_meal_time = get_time_ms();;
            pthread_mutex_unlock(&philo->table->meal_lock);
            release_forks(philo);
            if (philo->table->meals_required > 0 &&
                philo->meals_eaten >= philo->table->meals_required)
                return NULL;
            printf("[routine] Philosopher %i  Time since last meal: %ld ms (Time to Die Threshold: %i ms).",
                  philo->id, philo->last_meal_time, philo->table->time_to_die);

            print_status(philo, "is sleeping");
            usleep(philo->table->time_to_sleep * 1000);
            print_status(philo, "is thinking");

        } else
            wait_retry(philo, current_time);
    }
    return (NULL);
}

// Helper: wait depending on hunger before retrying fork acquisition
void wait_retry(t_philosopher *philo, long current_time)
{
    long time_remaining = philo->table->time_to_die - (current_time - philo->last_meal_time);

    // If the philosopher has more than 5 seconds left before dying, wait longer to avoid busy-waiting
    if (time_remaining > 5000)
    {
        usleep(5000);  // 5ms delay
    }
    else
    {
        // Otherwise, retry with a shorter delay, depending on how close they are to death
        usleep(time_remaining / 5);  // Divide by 5 to make the retry time proportional
    }
}