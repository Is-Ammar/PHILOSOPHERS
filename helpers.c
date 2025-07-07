/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:46:37 by iammar            #+#    #+#             */
/*   Updated: 2025/06/24 13:57:59 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

double	ft_atoi(const char *str)
{
	t_atoi	strr;

	strr = (t_atoi){0};
	strr.d = 1;
	strr.s = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			strr.s = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		strr.r = strr.r * 10 + (*str++ - '0');
	
	return (strr.s * strr.r);
}

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int		is_philosopher_dead(t_philo *philo)
{
	long	current_time;
	long	time_since_meal;

	pthread_mutex_lock(&philo->args->meal_mutex);
	current_time = get_current_time();
	time_since_meal = current_time - philo->last_meal_time;
	pthread_mutex_unlock(&philo->args->meal_mutex);
	
	return (time_since_meal > philo->time_to_die);
}

int		all_philosophers_ate(t_philo *head)
{
	t_philo	*current;
	int		i;

	if (!head || head->number_eat <= 0)
		return (0);
	
	current = head;
	i = 0;
	while (i < head->args->number_of_philosophers)
	{
		pthread_mutex_lock(&head->args->meal_mutex);
		if (current->meals_eaten < head->number_eat)
		{
			pthread_mutex_unlock(&head->args->meal_mutex);
			return (0);
		}
		pthread_mutex_unlock(&head->args->meal_mutex);
		current = current->next;
		i++;
	}
	return (1);
}

void	*monitor(void *arg)
{
	t_philo	*head = (t_philo *)arg;
	t_philo	*current;
	int		i;

	while (1)
	{
		pthread_mutex_lock(&head->args->death_mutex);
		if (head->args->is_dead)
		{
			pthread_mutex_unlock(&head->args->death_mutex);
			break;
		}
		pthread_mutex_unlock(&head->args->death_mutex);

		if (all_philosophers_ate(head))
		{
			pthread_mutex_lock(&head->args->death_mutex);
			head->args->all_ate = 1;
			pthread_mutex_unlock(&head->args->death_mutex);
			break;
		}

		current = head;
		i = 0;
		while (i < head->args->number_of_philosophers)
		{
			if (is_philosopher_dead(current))
			{
				pthread_mutex_lock(&head->args->mutex);
				printf("%ld %d died\n", 
					get_current_time() - head->args->start_time, current->id);
				pthread_mutex_unlock(&head->args->mutex);
				
				pthread_mutex_lock(&head->args->death_mutex);
				head->args->is_dead = 1;
				pthread_mutex_unlock(&head->args->death_mutex);
				return (NULL);
			}
			current = current->next;
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
