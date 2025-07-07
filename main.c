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
    philo->last_meal_time = get_current_time();
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
    
    while (1)
    {
        pthread_mutex_lock(&philo->args->death_mutex);
        if (philo->args->is_dead || philo->args->all_ate)
        {
            pthread_mutex_unlock(&philo->args->death_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->args->death_mutex);

        // Eating
        pthread_mutex_lock(&philo->args->mutex);
        printf("%ld %d is eating\n", 
            get_current_time() - philo->args->start_time, philo->id);
        pthread_mutex_unlock(&philo->args->mutex);
        
        pthread_mutex_lock(&philo->args->meal_mutex);
        philo->last_meal_time = get_current_time();
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->args->meal_mutex);
        
        usleep(philo->time_to_eat * 1000);

        // Check if this philosopher has eaten enough
        if (philo->number_eat > 0 && philo->meals_eaten >= philo->number_eat)
        {
            break;
        }

        // Sleeping
        pthread_mutex_lock(&philo->args->mutex);
        printf("%ld %d is sleeping\n", 
            get_current_time() - philo->args->start_time, philo->id);
        pthread_mutex_unlock(&philo->args->mutex);
        
        usleep(philo->time_to_sleep * 1000);

        // Thinking
        pthread_mutex_lock(&philo->args->mutex);
        printf("%ld %d is thinking\n", 
            get_current_time() - philo->args->start_time, philo->id);
        pthread_mutex_unlock(&philo->args->mutex);
        
        usleep(1000);
    }
    return NULL;
}

int main(int ac, char **av)
{
    t_philo *head = NULL;
    t_philo *philo;
    t_philo *current;
    t_args args = {0};
    pthread_t monitor_thread;
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
        else
            args.number_of_times_each_philosopher_must_eat = -1;
    }
    
    args.is_dead = 0;
    args.all_ate = 0;
    args.start_time = get_current_time();
    
    pthread_mutex_init(&args.mutex, NULL);
    pthread_mutex_init(&args.death_mutex, NULL);
    pthread_mutex_init(&args.meal_mutex, NULL);
    
    while(i <= args.number_of_philosophers)
    {
        philo = create_philosopher(i , &args);
        set_on_table(&head, philo);
        i++;
    }
    
    // Start all philosopher threads
    current = head;
    i = 0;
    while(i < args.number_of_philosophers)
    {
        pthread_create(&current->thread, NULL, routine, current);
        current = current->next;
        i++;
    }
    
    // Start monitor thread
    pthread_create(&monitor_thread, NULL, monitor, head);
    
    // Wait for monitor to finish
    pthread_join(monitor_thread, NULL);
    
    // Wait for all philosopher threads to finish
    current = head;
    i = 0;
    while(i < args.number_of_philosophers)
    {
        pthread_join(current->thread, NULL);
        current = current->next;
        i++;
    }
    
    pthread_mutex_destroy(&args.mutex);
    pthread_mutex_destroy(&args.death_mutex);
    pthread_mutex_destroy(&args.meal_mutex);
    
    return 0;
}
