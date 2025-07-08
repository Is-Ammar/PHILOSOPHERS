/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:43:39 by iammar            #+#    #+#             */
/*   Updated: 2025/07/08 09:58:24 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int all_philosophers_ate(t_philo *head, int total_philos)
{
    t_philo *current = head;
    int count = 0;
    int i = 0;
    
    if (head->args->number_of_times_each_philosopher_must_eat <= 0)
        return 0;
    while (i < total_philos)
    {
        if (current->meals_eaten >= current->args->number_of_times_each_philosopher_must_eat)
            count++;
        current = current->next;
        i++;
    }
    return (count == total_philos);
}

long long get_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void *monitor(void *arg)
{
    t_args *args = (t_args *)arg;
    t_philo *current;
    int i;
    long long current_time;
    
    while (args->simulation_running)
    {
        current = args->philosophers_head;
        i = 0;
        
        pthread_mutex_lock(&args->second);
        while (i < args->number_of_philosophers && args->simulation_running)
        {
            current_time = get_timestamp();
            pthread_mutex_lock(&args->mutex);
            if ((current_time - current->last_meal_time) >= current->time_to_die)
            {
                pthread_mutex_lock(&args->print_mutex);
                printf("%lld %d died\n", current_time - args->start_time, current->id);
                args->simulation_running = 0;
                pthread_mutex_unlock(&args->print_mutex);
                pthread_mutex_unlock(&args->mutex);
                return NULL;
            }
            pthread_mutex_unlock(&args->mutex);
            if (args->number_of_times_each_philosopher_must_eat > 0 && 
                all_philosophers_ate(args->philosophers_head, args->number_of_philosophers))
            {
                pthread_mutex_lock(&args->mutex);
                args->all_ate = 1;
                args->simulation_running = 0;
                pthread_mutex_unlock(&args->mutex);
                return NULL;
            }
            
            current = current->next;
            i++;
        }
        pthread_mutex_unlock(&args->second);

    }
    return NULL;
}
