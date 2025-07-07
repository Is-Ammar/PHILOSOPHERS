/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:25:47 by iammar            #+#    #+#             */
/*   Updated: 2025/06/25 18:37:40 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


t_philo *create_philosopher(int id, t_args *args)
{
    t_philo *philo;
    
    philo = malloc(sizeof(t_philo));
    if (!philo)
        return NULL;
    
    philo->id = id;
    philo->time_to_die = args->time_to_die;
    philo->time_to_eat = args->time_to_eat;
    philo->time_to_sleep = args->time_to_sleep;
    philo->number_eat = args->number_of_times_each_philosopher_must_eat;
    philo->meals_eaten = 0;
    philo->next = NULL;
    
    philo->args = args;
    return philo;
}

void set_on_table(t_philo **head, t_philo *philo)
{
    t_philo *curr;

    if (!*head)
    {
        *head = philo;
        (*head)->next = *head;
        return;
    }
    
    curr = *head;
    while (curr->next != *head)
        curr = curr->next;
    
    curr->next = philo;
    philo->next = *head;
}

void *routine(void *philos)
{
    t_philo *philo = (t_philo *)philos;
    pthread_mutex_lock(&philo->args->mutex);
    printf("%d  is eating\n", philo->id);
    pthread_mutex_unlock(&philo->args->mutex);
    return NULL;
}

int main(int ac, char **av)
{
    t_philo *head = NULL;
    t_philo *philo;
    t_args args = {0};
    int i = 1;

    if (ac < 5 || ac > 6)
    {
        write(2, "invalid arguments\n", 18);
        exit(1);
    }
    else
    {
        args.number_of_philosophers = ft_atoi(av[1]);
        args.time_to_die = ft_atoi(av[2]);
        args.time_to_eat = ft_atoi(av[3]);
        args.time_to_sleep = ft_atoi(av[4]);
        if (ac == 6)
            args.number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
    }
    while(i <= args.number_of_philosophers)
    {
        philo = create_philosopher(i , &args);
        set_on_table(&head, philo);
        i++;
    }
    pthread_mutex_init(&args.mutex, NULL);
    while(head && i > 1)
    {
        pthread_create(&head->thread, NULL, routine, head);
        pthread_join(head->thread, NULL);
        i--;
        head = head->next;
    }
    return 0;
}
