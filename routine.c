/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 21:13:21 by iammar            #+#    #+#             */
/*   Updated: 2025/07/07 22:34:36 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void *monitoring(void *arg)
{
    t_args *args = (t_args *)arg;
    t_philo *current;
    int i;
    long long current_time;
    while (args->simulation_running)
    {
        current = args->philosophers_head;
        i = 0;
        
        while (i < args->number_of_philosophers && args->simulation_running)
        {
            current_time = get_timestamp();
            pthread_mutex_lock(&args->mutex);
            if ((current_time - current->last_meal_time) > current->time_to_die)
            {
                pthread_mutex_lock(&args->print_mutex);
                printf("%lld %d died\n", current_time - args->start_time, current->id);
                args->simulation_running = 0;
                pthread_mutex_unlock(&args->print_mutex);
            }
            pthread_mutex_unlock(&args->mutex);
            if (all_philosophers_ate(args->philosophers_head, args->number_of_philosophers))
            {
                pthread_mutex_lock(&args->mutex);
                args->all_ate = 1;
                args->simulation_running = 0;
                pthread_mutex_unlock(&args->mutex);
            }
            
            current = current->next;
            i++;
        }
    }
    return NULL;
}
