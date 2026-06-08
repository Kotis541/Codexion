/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vokotera <vokotera@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 09:47:13 by vokotera          #+#    #+#             */
/*   Updated: 2026/06/08 09:47:13 by vokotera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	peek_heap_coder_id(t_dongle *dongle)
{
	if (dongle->heap_size > 0)
		return (dongle->heap_data[0].coder_id);
	return (-1);
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
	int				parent;

	i = dongle->heap_size;
	dongle->heap_data[i].coder_id = coder_id;
	dongle->heap_data[i].priority = priority;
	dongle->heap_size++;
	while (i != 0)
	{
		parent = (i - 1) / 2;
		if (dongle->heap_data[parent].priority > dongle->heap_data[i].priority)
		{
			swap_req(&dongle->heap_data[parent], &dongle->heap_data[i]);
			i = parent;
		}
		else
			break ;
	}
}

static void	bubble_down(t_dongle *dongle, int i)
{
	int				smallest;
	int				left;
	int				right;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < dongle->heap_size && dongle->heap_data[left].priority
			< dongle->heap_data[smallest].priority)
			smallest = left;
		if (right < dongle->heap_size && dongle->heap_data[right].priority
			< dongle->heap_data[smallest].priority)
			smallest = right;
		if (smallest == i)
			break ;
		swap_req(&dongle->heap_data[i], &dongle->heap_data[smallest]);
		i = smallest;
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
