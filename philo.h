/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:26:06 by iammar            #+#    #+#             */
/*   Updated: 2025/06/25 18:09:54 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H 
# define PHILO_H
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_atoi
{
	double	r;
	double	f;
	double	d;
	int		s;
}			t_atoi;

typedef struct s_args
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    pthread_mutex_t mutex;
    pthread_mutex_t death_mutex;
    pthread_mutex_t meal_mutex;
    int is_dead;
    int all_ate;
    long start_time;
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
    long last_meal_time;
    t_args *args;
    struct s_philo *next;
} t_philo;

double	ft_atoi(const char *str);
long	get_current_time(void);
int		is_philosopher_dead(t_philo *philo);
int		all_philosophers_ate(t_philo *head);
void	*monitor(void *arg);

#endif
