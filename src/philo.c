/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:31:53 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 10:51:58 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

static int	handle_special_args(char *arg);

static int	handle_special_args(char *arg)
{
	if (ft_strncmp(arg, "--help", 6) == 0)
	{
		write(1, "Usage: ./philo num_philos time_to_die time_to_eat time_to_sleep [num_meals]\n", 77);
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp(arg, "--debug", 7) == 0)
	{
		enable_debug_mode();
		log_output("Debug mode enabled");
		return (EXIT_SUCCESS);
	}
	write(2, "Invalid argument. Type --help\n", 31);
	return (EXIT_FAILURE);
}

int	main(int argc, char *argv[])
{
	t_table	table;

	if (argc == 2)
		return (handle_special_args(argv[1]));
	if (argc < 5 || argc > 6)
	{
		write(2, "Error: Invalid number of arguments. Type --help\n", 48);
		return (EXIT_FAILURE);
	}
	if (init_table(&table, argc, argv) == -1)
	{
		write(2, "Error: Initialization failed\n", 29);
		return (EXIT_FAILURE);
	}
	//start_simulation(&table);
	free_table(&table);
	return (EXIT_SUCCESS);
}


