/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:27 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/08 09:47:27 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_dongle(t_coder *coder, t_dongle *dongle, long priority)
{
	long	current_time;

	pthread_mutex_lock(&dongle->lock);
	push_to_heap(dongle, coder->id, priority);
	while (1)
	{
		if (check_death(coder))
		{
			pop_from_heap(dongle);
			pthread_mutex_unlock(&dongle->lock);
			return (0);
		}
		current_time = get_time();
		if (peek_heap_coder_id(dongle) == coder->id)
		{
			if (dongle->is_taken == 0 && current_time >= dongle->cooldown_end)
				break ;
			if (dongle->is_taken == 0 && current_time < dongle->cooldown_end)
			{
				pthread_mutex_unlock(&dongle->lock);
				usleep((dongle->cooldown_end - current_time) * 1000);
				pthread_mutex_lock(&dongle->lock);
				continue ;
			}
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
	pop_from_heap(dongle);
	dongle->is_taken = 1;
	pthread_mutex_unlock(&dongle->lock);
	return (1);
}

void	release_dongle(t_dongle *dongle, long cooldown_ms)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->is_taken = 0;
	dongle->cooldown_end = get_time() + cooldown_ms;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}
