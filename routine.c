#include "codexion.h"


int	check_death(t_coder *coder) 
{
	int status;

	pthread_mutex_lock(&coder->data->dead_mutex);
	status = coder->data->is_dead;
	pthread_mutex_unlock(&coder->data->dead_mutex);
	return status;
}

long	get_time() 
{
	struct	timeval tv;
	long	current_time;

	gettimeofday(&tv, NULL);
	current_time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return current_time;
}

void	*monitor_routine(void *args)
{

	t_coder *coders;
	int i;
	long last_compile_time;

	coders = (t_coder *)args;
	while (1)
	{
		i = 0;
		while (i < coders->data->number_of_coders)
		{
			pthread_mutex_lock(&coders[i].time_mutex);
			last_compile_time = coders[i].last_compile_time;
			pthread_mutex_unlock(&coders[i].time_mutex);

			if ((get_time() - last_compile_time >= coders[i].data->time_to_burnout))
			{
				pthread_mutex_lock(&coders[i].data->dead_mutex);
				coders[i].data->is_dead = 1;
				pthread_mutex_unlock(&coders[i].data->dead_mutex);
				pthread_mutex_lock(&coders[i].data->log_mutex);
				printf("%ld %d burned out\n", get_time() - coders[i].data->start_time, coders[i].id);
				pthread_mutex_unlock(&coders[i].data->log_mutex);
				return (NULL);
			}
			i++;
		}
	}
}

void	*routine(void *args)
{
	t_coder *coder;
	coder = (t_coder *)args;

	while (check_death(coder) == 0)
	{
		pthread_mutex_lock(&coder->data->log_mutex);
		if (coder->data->number_of_coders * 2 <= coder->data->compiles)
		{
			pthread_mutex_unlock(&coder->data->log_mutex);
			break;
		}
		pthread_mutex_unlock(&coder->data->log_mutex);
		if (coder->id % 2 == 0)
		{
			pthread_mutex_lock(coder->left_dongle);
			printf("%ld %d has taken a dongle\n", get_time() - coder->data->start_time, coder->id);
			pthread_mutex_lock(coder->right_dongle);
		}
		else
		{
			pthread_mutex_lock(coder->right_dongle);
			printf("%ld %d has taken a dongle\n", get_time() - coder->data->start_time, coder->id);
			pthread_mutex_lock(coder->left_dongle);
		}
		printf("%ld %d has taken a dongle\n", get_time() - coder->data->start_time, coder->id);
		pthread_mutex_lock(&coder->time_mutex);
		coder->last_compile_time = get_time();
		pthread_mutex_unlock(&coder->time_mutex);
		pthread_mutex_lock(&coder->data->log_mutex);
		printf("%ld %d is compiling\n", get_time() - coder->data->start_time, coder->id);
		pthread_mutex_unlock(&coder->data->log_mutex);
		usleep(coder->data->time_to_compile * 1000);
		pthread_mutex_unlock(coder->left_dongle);
		pthread_mutex_unlock(coder->right_dongle);
		pthread_mutex_lock(&coder->data->log_mutex);
		coder->data->compiles++;
		pthread_mutex_unlock(&coder->data->log_mutex);
		pthread_mutex_lock(&coder->data->log_mutex);
		printf("%ld %d is debugging\n", get_time() - coder->data->start_time, coder->id);
		pthread_mutex_unlock(&coder->data->log_mutex);
		usleep(coder->data->time_to_debug * 1000);
		pthread_mutex_lock(&coder->data->log_mutex);
		printf("%ld %d is refactoring\n", get_time() - coder->data->start_time, coder->id);
		pthread_mutex_unlock(&coder->data->log_mutex);
		usleep(coder->data->time_to_refactor * 1000);
	}
	return (NULL);
}
