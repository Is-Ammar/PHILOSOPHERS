/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:45:14 by iammar            #+#    #+#             */
/*   Updated: 2025/07/27 09:37:30 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_ready(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->lock);
	while (!philo->args->simulation_running)
	{
		pthread_mutex_unlock(&philo->args->lock);
		usleep(10);
		pthread_mutex_lock(&philo->args->lock);
	}
	pthread_mutex_unlock(&philo->args->lock);
	pthread_mutex_lock(&philo->args->mutex);
	philo->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&philo->args->mutex);
	if (philo->id % 2 == 0)
		usleep(philo->args->time_to_eat * 500);
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->lock);
	if (!philo->args->simulation_running || philo->args->all_ate)
	{
		pthread_mutex_unlock(&philo->args->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->args->lock);
	if (philo->id < philo->next->id)
	{
		pthread_mutex_lock(&philo->fork);
		print(philo, "has taken a fork");
		pthread_mutex_lock(&philo->next->fork);
		print(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->next->fork);
		print(philo, "has taken a fork");
		pthread_mutex_lock(&philo->fork);
		print(philo, "has taken a fork");
	}
}

void	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->mutex);
	philo->last_meal_time = get_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->args->mutex);
	print(philo, "is eating");
	usleep(philo->args->time_to_eat * 1000);
}

void	*routine(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	get_ready(philo);
	while (1337)
	{
		pthread_mutex_lock(&philo->args->lock);
		if (!philo->args->simulation_running || philo->args->all_ate)
		{
			pthread_mutex_unlock(&philo->args->lock);
			break ;
		}
		pthread_mutex_unlock(&philo->args->lock);
		take_forks(philo);
		eating(philo);
		pthread_mutex_unlock(&philo->fork);
		pthread_mutex_unlock(&philo->next->fork);
		print(philo, "is sleeping");
		usleep(philo->args->time_to_sleep * 1000);
		print(philo, "is thinking");
		if (philo->args->number_of_philosophers % 2 != 0
			&& philo->args->time_to_eat >= philo->args->time_to_sleep)
			usleep(philo->args->time_to_eat * 1000);
	}
	return (NULL);
}
