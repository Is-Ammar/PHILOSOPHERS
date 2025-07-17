/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 02:46:07 by iammar            #+#    #+#             */
/*   Updated: 2025/07/17 13:45:12 by iammar           ###   ########.fr       */
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
	philo->time_to_die = args->time_to_die;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->number_eat = args->number_of_eats;
	philo->meals_eaten = 0;
	philo->is_dead = 0;
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
