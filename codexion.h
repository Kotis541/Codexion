#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct s_data
{
  int             number_of_coders;
  long            time_to_burnout;
  long            time_to_compile;
  long            time_to_debug;
  long            time_to_refactor;
  int             number_of_compiles_required;
  int             dongle_cooldown;
  int             scheduler;

  long            start_time;
  int             is_dead;
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




#endif