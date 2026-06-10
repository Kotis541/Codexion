/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:27 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/10 10:08:39 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_for_turn(t_coder *coder, t_dongle *dongle)
{
	long	current_time;

	while (1)
	{
		if (check_death(coder))
		{
			pop_from_heap(dongle);
			return (0);
		}
		current_time = get_time();
		if (peek_heap_coder_id(dongle) == coder->id && !dongle->is_taken)
		{
			if (current_time >= dongle->cooldown_end)
				return (1);
			pthread_mutex_unlock(&dongle->lock);
			usleep((dongle->cooldown_end - current_time) * 1000);
			pthread_mutex_lock(&dongle->lock);
			continue ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	}
}

int	request_dongle(t_coder *coder, t_dongle *dongle, long priority)
{
	pthread_mutex_lock(&dongle->lock);
	push_to_heap(dongle, coder->id, priority);
	if (!wait_for_turn(coder, dongle))
	{
		pthread_mutex_unlock(&dongle->lock);
		return (0);
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
