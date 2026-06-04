#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>


typedef struct s_data
{
  int             number_of_coders;
  long            time_to_burnout;
  long            time_to_compile;
  long            time_to_debug;
  long            time_to_refactor;
  int             number_of_compiles_required;
  int             compiles;
  int             dongle_cooldown;
  int             scheduler;
  long            start_time;
  int             counter;
  int             currently_serving;
  int             is_dead;
  pthread_cond_t  schedule_cond;
  pthread_mutex_t dead_mutex;
  pthread_mutex_t log_mutex;
  pthread_mutex_t *dongles;
} t_data;


typedef struct s_coder
{
  int             id;
  int             compiles_done;
  long            last_compile_time;
  pthread_mutex_t *left_dongle;
  pthread_mutex_t *right_dongle;
  pthread_mutex_t time_mutex;
  t_data          *data;
} t_coder;

long  get_time();
int   parse_arg(t_data *data, int argc, char **argv);
void	*routine(void *args);
void	*monitor_routine(void *args);
int   init_coders(t_data *data, t_coder **coders);
void	*monitor_routine(void *args);



#endif