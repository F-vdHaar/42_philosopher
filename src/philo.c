/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:31:53 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 12:21:17 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_special_args(char *arg);

static int	handle_special_args(char *arg)
{
	if (ft_strncmp(arg, "--help", 6) == 0)
	{
		write(1, "Usage: ./philo num_philos ", 27);
		write(1, "t_die time_to_eat time_to_sleep [num_meals]\n", 45);
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp(arg, "--debug", 7) == 0)
	{
		enable_debug_mode();
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

int	main(int argc, char *argv[])
{
	t_table	table;

	if (argv[1])
		handle_special_args(argv[1]);
	if (ft_strncmp(argv[1], "--debug", 7) == 0)
	{
		argc--;
		argv++;
	}
	if (argc < 5 || argc > 6)
	{
		write(2, "Error: Invalid number of arguments. Type --help\n", 48);
		return (EXIT_FAILURE);
	}
	if (init_table(&table, argc, argv) == -1)
	{
		log_error("[main]: Table Initialization failed");
		return (EXIT_FAILURE);
	}
	start_simulation(&table);
	free_table(&table);
	return (EXIT_SUCCESS);
}
