/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:43:39 by iammar            #+#    #+#             */
/*   Updated: 2025/07/17 14:23:59 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	all_philosophers_ate(t_philo *head, int total_philos)
{
	t_philo	*current;
	int		count;
	int		i;
	int		current_meals;

	current = head;
	count = 0;
	i = 0;
	if (head->args->number_of_eats <= 0)
		return (0);
	while (i < total_philos)
	{
		pthread_mutex_lock(&current->args->mutex);
		current_meals = current->meals_eaten;
		pthread_mutex_unlock(&current->args->mutex);
		if (current_meals >= current->args->number_of_eats)
			count++;
		current = current->next;
		i++;
	}
	return (count == total_philos);
}

long long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_death(t_args *args, t_philo *current)
{
	long long	current_time;

	current_time = get_timestamp();
	if ((current_time - current->last_meal_time) >= current->time_to_die)
	{
		pthread_mutex_lock(&args->print_mutex);
		printf("%lld %d died\n", current_time - args->start_time, current->id);
		pthread_mutex_unlock(&args->print_mutex);
		pthread_mutex_lock(&args->lock);
		args->simulation_running = 0;
		pthread_mutex_unlock(&args->lock);
		pthread_mutex_unlock(&args->mutex);
		pthread_mutex_unlock(&args->second);
		return (DEAD);
	}
	return (ALIVE);
}

int	check_finished(t_args *args)
{
	if (args->number_of_eats > 0
		&& all_philosophers_ate(args->philosophers_head,
			args->number_of_philosophers))
	{
		pthread_mutex_lock(&args->mutex);
		pthread_mutex_lock(&args->lock);
		args->all_ate = 1;
		args->simulation_running = 0;
		pthread_mutex_unlock(&args->mutex);
		pthread_mutex_unlock(&args->lock);
		pthread_mutex_unlock(&args->second);
		return (FINISHED);
	}
	return (CONTINUE);
}

void	*monitor(void *arg)
{
	t_args	*args;
	t_philo	*current;
	int		i;

	args = (t_args *)arg;
	while (args->simulation_running)
	{
		current = args->philosophers_head;
		i = 0;
		pthread_mutex_lock(&args->second);
		while (i <= args->number_of_philosophers && args->simulation_running)
		{
			pthread_mutex_lock(&args->mutex);
			if (check_death(args, current) == DEAD)
				return (NULL);
			pthread_mutex_unlock(&args->mutex);
			if (check_finished(args) == FINISHED)
				return (NULL);
			current = current->next;
			i++;
		}
		pthread_mutex_unlock(&args->second);
	}
	return (NULL);
}
