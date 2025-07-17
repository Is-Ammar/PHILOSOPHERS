/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 03:54:03 by iammar            #+#    #+#             */
/*   Updated: 2025/07/17 05:32:47 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void print(t_philo *philo, char *message)
{
    long long current_time;
    
    pthread_mutex_lock(&philo->args->print_mutex);
    pthread_mutex_lock(&philo->args->lock);
    if (philo->args->simulation_running)
    {
        current_time = get_timestamp();
        printf("%lld %d %s\n", current_time - philo->args->start_time, philo->id + 1, message);
    }
    pthread_mutex_unlock(&philo->args->lock);
    pthread_mutex_unlock(&philo->args->print_mutex);
}
