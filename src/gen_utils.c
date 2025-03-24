/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:51:18 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/24 10:21:33 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	g_debug_mode = 0;

void	log_error(const char *message)
{
	int	fd;

	printf("Error: %s\n", message);
	fd = open("error_log.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		printf("Error: Failed to open log file\n");
		return ;
	}
	printf("Error: %s\n", message);
	close(fd);
}

void	log_output(const char *message)
{
	const char	*debug_prefix;

	if (g_debug_mode)
	{
		debug_prefix = "[DEBUG] ";
		write(STDERR_FILENO, debug_prefix, 8);
		while (*message)
		{
			write(STDERR_FILENO, message, 1);
			message++;
		}
		write(STDERR_FILENO, "\n", 1);
	}
}

void	enable_debug_mode(void)
{
	g_debug_mode = 1;
	write(2, "[DEBUG] Debug mode enabled\n", 26);
	log_output("Debug mode enabled\n");
}
