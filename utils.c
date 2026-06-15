/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:30 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/15 13:11:26 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long time_in_ms, t_coder *coder)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
	{
		if (check_death(coder))
			break ;
		usleep(500);
	}
}

int	check_death(t_coder *coder)
{
	int	status;

	pthread_mutex_lock(&coder->data->dead_mutex);
	status = coder->data->is_dead;
	pthread_mutex_unlock(&coder->data->dead_mutex);
	return (status);
}

void	print_status(t_coder *coder, char *msg)
{
	long	time_diff;

	pthread_mutex_lock(&coder->data->log_mutex);
	time_diff = get_time() - coder->data->start_time;
	if (!check_death(coder))
		printf("%ld %d %s\n", time_diff, coder->id, msg);
	pthread_mutex_unlock(&coder->data->log_mutex);
}
