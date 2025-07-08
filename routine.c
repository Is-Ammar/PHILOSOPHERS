/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:45:14 by iammar            #+#    #+#             */
/*   Updated: 2025/07/08 03:00:43 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *routine(void *philos)
{
    t_philo *philo = (t_philo *)philos;

    philo->last_meal_time = get_timestamp();
    
    if (philo->id % 2 == 0)
        usleep(1000);
    while(philo->args->simulation_running)
    {
        if (!philo->args->simulation_running || philo->args->all_ate)
            break;
        if((philo->id % 2) == 1)
        {
            pthread_mutex_lock(&philo->fork);
            safe_print(philo, "has taken a fork");
            pthread_mutex_lock(&philo->next->fork);
            safe_print(philo, "has taken a fork");
        }
        else
        {
            pthread_mutex_lock(&philo->next->fork);
            safe_print(philo, "has taken a fork");
            pthread_mutex_lock(&philo->fork);
            safe_print(philo, "has taken a fork");
        }
        pthread_mutex_lock(&philo->args->mutex);
        philo->last_meal_time = get_timestamp();
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->args->mutex);
        
        safe_print(philo, "is eating");
        usleep(philo->time_to_eat * 1000);
        
        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);
        
        safe_print(philo, "is sleeping");
        usleep(philo->time_to_sleep * 1000);
        
        safe_print(philo, "is thinking");
        if(philo->args->number_of_philosophers % 2 && philo->time_to_eat <= philo->time_to_sleep)
            usleep(philo->time_to_eat * 1000);
    }
    return NULL;
}
