#include "codexion.h"


int init_coders(t_data *data, t_coder **coders)
{
	int i;

	*coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!(*coders))
		return (1);
	
	i = 0;
	while (i < data->number_of_coders)
	{
		(*coders)[i].id = i + 1;
		(*coders)[i].data = data;
		(*coders)[i].last_compile_time = get_time();
		pthread_mutex_init(&(*coders)[i].time_mutex, NULL);
		(*coders)[i].left_dongle = &data->dongles[i];
		(*coders)[i].right_dongle = &data->dongles[(i + 1) % data->number_of_coders];
		i++;
	}
	return 0;
}

int main(int argc, char **argv)
{
	t_data 		*data;
	t_coder		*coders;
	pthread_t   *threads;
	pthread_t 	monitor;
	int i;

	data = malloc(sizeof(t_data));
	if (!data)
		return 1;
	if (parse_arg(data, argc, argv) != 0)
		return 1;
	data->start_time = get_time();
	if (init_coders(data, &coders) != 0)
		return 1;
	data->start_time = get_time();
	data->counter = 0;
	data->currently_serving = 0;
	data->compiles = 0;
	threads = malloc(sizeof(pthread_t) * data->number_of_coders);

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_create(&threads[i], NULL, &routine, &coders[i]);
		i++;
	}
	pthread_create(&monitor, NULL, &monitor_routine, coders);
	pthread_join(monitor, NULL);
	
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
    free(coders);
    free(data->dongles);
    free(data);
	return (0);
}