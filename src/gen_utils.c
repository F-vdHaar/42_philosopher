/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvon-de <fvon-der@student.42heilbronn.d    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:51:18 by fvon-de           #+#    #+#             */
/*   Updated: 2025/03/25 12:26:53 by fvon-de          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h> 
#include <fcntl.h> 
#include <unistd.h>

int	g_debug_mode = 0;

void	log_error(const char *message)
{
	int			fd;
	const char	*prefix = "Error: ";

	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
	fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		write(STDERR_FILENO, "Error: Failed to open log file\n", 31);
		return ;
	}
	write(fd, prefix, ft_strlen(prefix));
	write(fd, message, ft_strlen(message));
	write(fd, "\n", 1);
	close(fd);
}

void	log_output(const char *message)
{
	int			fd;
	const char	*prefix = "[DEBUG] ";

	if (g_debug_mode)
	{
		write(STDERR_FILENO, prefix, ft_strlen(prefix));
		write(STDERR_FILENO, message, ft_strlen(message));
		write(STDERR_FILENO, "\n", 1);
		fd = open(LOG_FILE, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd == -1)
		{
			write(STDERR_FILENO, "Error: Failed to open log file\n", 31);
			return ;
		}
		write(fd, prefix, ft_strlen(prefix));
		write(fd, message, ft_strlen(message));
		write(fd, "\n", 1);
		close(fd);
	}
}

void	enable_debug_mode(void)
{
	g_debug_mode = 1;
	log_output("Debug mode enabled\n");
}

/* Utility to get current timestamp in milliseconds */
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
