/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:26:06 by iammar            #+#    #+#             */
/*   Updated: 2025/07/17 05:05:20 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H 
# define PHILO_H
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef enum s_macros
{
   ALIVE,
   DEAD,
   FINISHED,
   CONTINUE,
}t_macros;

typedef struct s_atoi
{
	double	r;
	double	f;
	double	d;
	int		s;
}			t_atoi;

typedef struct s_args t_args;
typedef struct s_philo t_philo;
typedef struct s_args
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    int all_ate;
    int simulation_running;
    long long start_time;
    t_philo *philosophers_head;
    pthread_mutex_t print_mutex;
    pthread_mutex_t second;
    int             can_start;
    pthread_mutex_t start_mutex; 
    pthread_mutex_t mutex;
    pthread_mutex_t lock;
} t_args;

typedef struct s_philo
{
    pthread_t thread;
    int id;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_eat;
    int meals_eaten;
    long long last_meal_time;
    int is_dead;
    pthread_mutex_t fork;
    int has_eaten_enough;;
    t_args *args;
    struct s_philo *next;
} t_philo;

double	ft_atoi(const char *str);
void *monitor(void *arg);
long long get_timestamp();
void *routine(void *philos);
t_philo *create_philosopher(int id, t_args *args);
void set_on_table(t_philo **head, t_philo *philo);
void print(t_philo *philo, char *message);

#endif
