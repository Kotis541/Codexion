/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:13 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/15 13:55:12 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_higher(t_dongle_req *a, t_dongle_req *b)
{
	if (a->priority < b->priority)
		return (1);
	if (a->priority == b->priority)
		return (a->coder_id < b->coder_id);
	return (0);
}

static void	swap_req(t_dongle_req *a, t_dongle_req *b)
{
	t_dongle_req	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	push_to_heap(t_dongle *dongle, int coder_id, long priority)
{
	int				i;
	int				p;
	t_dongle_req	*d;

	i = dongle->heap_size;
	d = dongle->heap_data;
	d[i].coder_id = coder_id;
	d[i].priority = priority;
	dongle->heap_size++;
	while (i != 0)
	{
		p = (i - 1) / 2;
		if (is_higher(&d[i], &d[p]))
		{
			swap_req(&d[p], &d[i]);
			i = p;
		}
		else
			break ;
	}
}

static void	bubble_down(t_dongle *dongle, int i)
{
	int				s;
	int				l;
	int				r;
	t_dongle_req	*d;

	d = dongle->heap_data;
	while (1)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		s = i;
		if (l < dongle->heap_size && is_higher(&d[l], &d[s]))
			s = l;
		if (r < dongle->heap_size && is_higher(&d[r], &d[s]))
			s = r;
		if (s == i)
			break ;
		swap_req(&d[i], &d[s]);
		i = s;
	}
}

void	pop_from_heap(t_dongle *dongle)
{
	if (dongle->heap_size <= 0)
		return ;
	dongle->heap_size--;
	if (dongle->heap_size == 0)
		return ;
	dongle->heap_data[0] = dongle->heap_data[dongle->heap_size];
	bubble_down(dongle, 0);
}
