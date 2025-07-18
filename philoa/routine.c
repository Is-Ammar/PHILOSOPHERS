/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:45:14 by iammar            #+#    #+#             */
/*   Updated: 2025/07/18 11:42:25 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_ready(t_philo *philo)
{
	int	can_start;

	can_start = 0;
	while (!can_start)
	{
		pthread_mutex_lock(&philo->args->lock);
		can_start = philo->args->simulation_running;
		pthread_mutex_unlock(&philo->args->lock);
		if (!can_start)
			usleep(20);
	}
	pthread_mutex_lock(&philo->args->mutex);
	philo->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&philo->args->mutex);
	if (philo->id % 2 == 0)
		usleep(philo->time_to_eat * 1000 / 2);
}

void	take_forks(t_philo *philo)
{
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
	print(philo, "is eating");
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_lock(&philo->args->mutex);
	philo->last_meal_time = get_timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->args->mutex);
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
		usleep(philo->time_to_sleep * 1000);
		print(philo, "is thinking");
		if (philo->args->number_of_philosophers % 2 != 0
			&& philo->time_to_eat >= philo->time_to_sleep)
			usleep(philo->time_to_eat * 1000);
	}
	return (NULL);
}
