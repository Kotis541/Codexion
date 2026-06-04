#include "codexion.h"

int    check_arg(int argc, char **argv)
{
    if (argc > 9)
    {
        printf("ERROR: You added too many arguments\n");
        return 1;
    }
    else if (argc < 9)
    {
        printf("ERROR: You need to add more arguments\n");
        return 1;
    }
    int i = 1;
    while (i < argc - 1)
    {
        int j = 0;
        while (argv[i][j])
        {
            if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
            {
                printf("ERROR: You can add just numbers!\n");
                return 1;
            }
            j++;
        }
        i++;
    }
    return 0;
}

int parse_arg(t_data *data, int argc, char **argv)
{
    if (check_arg(argc, argv))
        return 1;
    if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
    {
        printf("ERROR: You added wrong scheduler (fifo or edf)\n");
        return 1;
    }
    data->number_of_coders = atoi(argv[1]);
    data->time_to_burnout = atoi(argv[2]);
    data->time_to_compile = atoi(argv[3]);
    data->time_to_debug = atoi(argv[4]);
    data->time_to_refactor = atoi(argv[5]);
    data->number_of_compiles_required = atoi(argv[6]);
    data->dongle_cooldown = atoi(argv[7]);
    data->compiles = 0;
    if (strcmp(argv[8], "edf") == 0)
        data->scheduler = 1;
    else
        data->scheduler = 0;
    data->start_time = 0;
    data->is_dead = 0;
    pthread_mutex_init(&data->dead_mutex, NULL);
    pthread_mutex_init(&data->log_mutex, NULL);
    pthread_cond_init(&data->schedule_cond, NULL);
    data->dongles = malloc(sizeof(pthread_mutex_t) * data->number_of_coders);
    if (!data->dongles)
        return 1;
    
    int i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_init(&data->dongles[i], NULL);
        i++;
    }
    return 0;
}