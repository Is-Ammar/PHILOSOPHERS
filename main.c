/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iammar <iammar@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:25:47 by iammar            #+#    #+#             */
/*   Updated: 2025/07/08 02:47:15 by iammar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void safe_print(t_philo *philo, char *message)
{
    pthread_mutex_lock(&philo->args->print_mutex);
    if (philo->args->simulation_running)
        printf("%lld %d %s\n", get_timestamp() - philo->args->start_time, philo->id, message);
    pthread_mutex_unlock(&philo->args->print_mutex);
}

int main(int ac, char **av)
{
    t_philo *head = NULL;
    t_philo *philo;
    t_args args = {0};
    int i = 1;
    pthread_t monitor_thread;
    t_philo *current;

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
    args.all_ate = 0;
    args.simulation_running = 1;
    args.start_time = get_timestamp();
    pthread_mutex_init(&args.mutex, NULL);
    pthread_mutex_init(&args.print_mutex, NULL);
    
    while(i <= args.number_of_philosophers)
    {
        philo = create_philosopher(i, &args);
        set_on_table(&head, philo);
        i++;
    }
    args.philosophers_head = head;
    current = head;
    i = args.number_of_philosophers;
    while(i > 0)
    {
        pthread_create(&current->thread, NULL, routine, current);
        current = current->next;
        i--;
    }
    pthread_create(&monitor_thread, NULL, monitor, &args);
    current = head;
    i = args.number_of_philosophers;
    while(i > 0)
    {
        pthread_join(current->thread, NULL);
        current = current->next;
        i--;
    }
    pthread_join(monitor_thread, NULL);
    
    return 0;
}