/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:46:07 by iammar            #+#    #+#             */
/*   Updated: 2025/07/26 17:17:42 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*create_philosopher(int id, t_args *args)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->meals_eaten = 0;
	pthread_mutex_init(&philo->fork, NULL);
	philo->last_meal_time = get_timestamp();
	philo->next = NULL;
	philo->args = args;
	return (philo);
}

void	set_on_table(t_philo **head, t_philo *philo)
{
	t_philo	*curr;

	if (!*head)
	{
		*head = philo;
		(*head)->next = *head;
		return ;
	}
	curr = *head;
	while (curr->next != *head)
		curr = curr->next;
	curr->next = philo;
	philo->next = *head;
}

void	destroy_mutex(t_args *args)
{
	pthread_mutex_destroy(&args->mutex);
	pthread_mutex_destroy(&args->print_mutex);
	pthread_mutex_destroy(&args->lock);
	pthread_mutex_destroy(&args->second);
}

void	init_mutex(t_args *args)
{
	pthread_mutex_init(&args->mutex, NULL);
	pthread_mutex_init(&args->print_mutex, NULL);
	pthread_mutex_init(&args->lock, NULL);
	pthread_mutex_init(&args->second, NULL);
}

void	clear_philo(t_philo *philo, int i)
{
	t_philo	*nxt;

	while (philo && i-- > 0)
	{
		nxt = philo->next;
		if (philo)
		{
			pthread_mutex_destroy(&philo->fork);
			free(philo);
			philo = NULL;
		}
		philo = nxt;
	}
}
